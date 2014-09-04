/* Copyright (C) 2014 InfiniDB, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of
   the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */

//  $Id: crossenginestep.cpp 9242 2013-02-01 13:31:57Z rdempsey $

#include <unistd.h>
//#define NDEBUG
#include <cassert>
#include <sstream>
#include <iomanip>
using namespace std;

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
using namespace boost;

#include "messagequeue.h"
using namespace messageqcpp;

#include "loggingid.h"
#include "errorcodes.h"
#include "idberrorinfo.h"
#include "errorids.h"
using namespace logging;

#include "calpontsystemcatalog.h"
#include "constantcolumn.h"
using namespace execplan;

#include "rowgroup.h"
using namespace rowgroup;

#include "dataconvert.h"
using namespace dataconvert;

#include "funcexp.h"

#include "jobstep.h"
#include "jlf_common.h"
#include "crossenginestep.h"

#include "libdrizzle-2.0/drizzle.h"
#include "libdrizzle-2.0/drizzle_client.h"

namespace joblist
{

CrossEngineStep::CrossEngineStep(
	const JobStepAssociation& inputJobStepAssociation,
	const JobStepAssociation& outputJobStepAssociation,
	uint32_t statementId,
	const string& schema,
	const string& table,
	const string& alias,
	const JobInfo& jobInfo) :
		fInputJobStepAssociation(inputJobStepAssociation),
		fOutputJobStepAssociation(outputJobStepAssociation),
		fStepId(0),
		fRowsRetrieved(0),
		fRowsReturned(0),
		fRowsPerGroup(256),
		fOutputDL(NULL),
		fOutputIterator(0),
		fEndOfResult(false),
		fDelivery(false),
		fSchema(schema),
		fTable(table),
		fAlias(alias),
		fColumnCount(0),
		fFeInstance(funcexp::FuncExp::instance())
{
	fExtendedInfo = "CES: ";
	getMysqldInfo(jobInfo);
}


CrossEngineStep::~CrossEngineStep()
{
}


void CrossEngineStep::setOutputRowGroup(const rowgroup::RowGroup& rg)
{
	fRowGroupOut = fRowGroupDelivered = rg;
}


void CrossEngineStep::addFcnExpGroup1(const boost::shared_ptr<ParseTree>& fe)
{
	fFeFilters = fe;
}


void CrossEngineStep::setFE1Input(const rowgroup::RowGroup& rg)
{
	fRowGroupFe1 = rg;
}


void CrossEngineStep::setFcnExpGroup3(const vector<shared_ptr<ReturnedColumn> >& fe)
{
	fFeSelects = fe;
}


void CrossEngineStep::setFE23Output(const rowgroup::RowGroup& rg)
{
	fRowGroupFe3 = fRowGroupDelivered = rg;
}


void CrossEngineStep::makeMappings()
{
	fFe1Column.reset(new int[fColumnCount]);
	for (uint64_t i = 0; i < fColumnCount; ++i)
		fFe1Column[i] = -1;
			
	if (fFeFilters != NULL)
	{
		const std::vector<uint>& colInFe1 = fRowGroupFe1.getKeys();
		for (uint64_t i = 0; i < colInFe1.size(); i++)
		{
			map<uint, uint>::iterator it = fColumnMap.find(colInFe1[i]);
			if (it != fColumnMap.end())
				fFe1Column[it->second] = i;
			else
				throw logic_error("Column not projected.");
		}

		fFeMapping1 = makeMapping(fRowGroupFe1, fRowGroupOut);
	}

	if (!fFeSelects.empty())
		fFeMapping3 = makeMapping(fRowGroupOut, fRowGroupFe3);
}


void CrossEngineStep::setField(int i, const char* value, Row& row)
{
	CalpontSystemCatalog::ColDataType colType = row.getColType(i);

	if ((colType == CalpontSystemCatalog::CHAR || colType == CalpontSystemCatalog::VARCHAR) &&
		row.getColumnWidth(i) > 8)
	{
		if (value != NULL)
			row.setStringField(value, i);
		else
			row.setStringField("", i);
	}
	else
	{
		CalpontSystemCatalog::ColType ct;
		ct.colDataType = colType;
		ct.colWidth = row.getColumnWidth(i);
		ct.scale = row.getScale(i);
		ct.precision = row.getPrecision(i);
		row.setIntField(convertValueNum(value, ct, row.getSignedNullValue(i)), i);
	}
}


inline void CrossEngineStep::addRow(shared_array<uint8_t>& data)
{
	fRowDelivered.setRid(fRowsReturned%fRowsPerGroup);
	fRowDelivered.nextRow();
	fRowGroupDelivered.incRowCount();

	if (++fRowsReturned%fRowsPerGroup == 0)
	{
		fOutputDL->insert(data);
		data.reset(new uint8_t[fRowGroupDelivered.getDataSize(fRowsPerGroup)]);
		fRowGroupDelivered.setData(data.get());
		fRowGroupDelivered.resetRowGroup(fRowsReturned);
		fRowGroupDelivered.getRow(0, &fRowDelivered);
	}
}


// simplified version of convertValueNum() in jlf_execplantojoblist.cpp.
int64_t CrossEngineStep::convertValueNum(
	const char* str, const CalpontSystemCatalog::ColType& ct, int64_t nullValue)
{
	// return value
	int64_t rv = nullValue;

	// null value
	if (str == NULL)
		return rv;

	// convertColumnData(execplan::CalpontSystemCatalog::ColType colType,
	//                   const std::string& dataOrig,
	//                   bool& pushWarning,
	//                   bool nulFlag,
	//                   bool noRoundup )
	bool pushWarning = false;
	boost::any anyVal = DataConvert::convertColumnData(ct, str, pushWarning, false, true);

	// Out of range values are treated as NULL as discussed during design review.
	if (pushWarning)
		return rv;

	// non-null value
	switch (ct.colDataType)
	{
		case CalpontSystemCatalog::BIT:
		rv = boost::any_cast<bool>(anyVal);
			break;
		case CalpontSystemCatalog::TINYINT:
		rv = boost::any_cast<char>(anyVal);
			break;
		case CalpontSystemCatalog::SMALLINT:
		rv = boost::any_cast<int16_t>(anyVal);
			break;
		case CalpontSystemCatalog::MEDINT:
		case CalpontSystemCatalog::INT:
#ifdef _MSC_VER
		rv = boost::any_cast<int>(anyVal);
#else
		rv = boost::any_cast<int32_t>(anyVal);
#endif
			break;
		case CalpontSystemCatalog::BIGINT:
		rv = boost::any_cast<long long>(anyVal);
			break;
		case CalpontSystemCatalog::FLOAT:
			{
				float f = boost::any_cast<float>(anyVal);
				//N.B. There is a bug in boost::any or in gcc where, if you store a nan,
				//     you will get back a nan, but not necessarily the same bits that you put in.
				//     This only seems to be for float (double seems to work).
				if (isnan(f))
				{
					uint32_t ti = joblist::FLOATNULL;
					float* tfp = (float*)&ti;
					f = *tfp;
				}
				float* fp = &f;
				int32_t* ip = reinterpret_cast<int32_t*>(fp);
			  rv = *ip;
			}
			break;
		case CalpontSystemCatalog::DOUBLE:
			{
				double d = boost::any_cast<double>(anyVal);
				double* dp = &d;
				int64_t* ip = reinterpret_cast<int64_t*>(dp);
			  rv = *ip;
			}
			break;
		case CalpontSystemCatalog::CHAR:
		case CalpontSystemCatalog::VARCHAR:
		case CalpontSystemCatalog::VARBINARY:
		case CalpontSystemCatalog::BLOB:
		case CalpontSystemCatalog::CLOB:
			{
				string i = boost::any_cast<string>(anyVal);
				// bug 1932, pad nulls up to the size of v
				i.resize(sizeof(rv), 0);
			  rv = *((uint64_t *) i.data());
			}
			break;
		case CalpontSystemCatalog::DATE:
		rv = boost::any_cast<uint32_t>(anyVal);
			break;
		case CalpontSystemCatalog::DATETIME:
		rv = boost::any_cast<uint64_t>(anyVal);
			break;
		case CalpontSystemCatalog::DECIMAL:
			if (ct.colWidth == CalpontSystemCatalog::ONE_BYTE)
			rv = boost::any_cast<char>(anyVal);
			else if (ct.colWidth == CalpontSystemCatalog::TWO_BYTE)
			rv = boost::any_cast<int16_t>(anyVal);
			else if (ct.colWidth == CalpontSystemCatalog::FOUR_BYTE)
#ifdef _MSC_VER
			rv = boost::any_cast<int>(anyVal);
#else
			rv = boost::any_cast<int32_t>(anyVal);
#endif
			else 
			rv = boost::any_cast<long long>(anyVal);
			break;
		default:
			break;
	}

	return rv;
}


void CrossEngineStep::getMysqldInfo(const JobInfo& jobInfo)
{
	if (jobInfo.rm.getMysqldInfo(fHost, fUser, fPasswd, fPort) == false)
		throw IDBExcept(IDBErrorInfo::instance()->errorMsg(ERR_CROSS_ENGINE_CONFIG),
						ERR_CROSS_ENGINE_CONFIG);
}


void CrossEngineStep::run()
{
//	idbassert(!fDelivery);

	if (fOutputJobStepAssociation.outSize() == 0)
		throw logic_error("No output data list for non-delivery constant step.");

	fOutputDL = fOutputJobStepAssociation.outAt(0)->rowGroupDL();
	if (fOutputDL == NULL)
		throw logic_error("Output is not a RowGroup data list.");

    if (fDelivery == true)
    {
        fOutputIterator = fOutputDL->getIterator();
    }

	fRunner.reset(new boost::thread(Runner(this)));
}


void CrossEngineStep::join()
{
	if (fRunner)
		fRunner->join();
}


void CrossEngineStep::execute()
{
	drizzle_st* drzp=0;
	drzp = drizzle_create();
	if (drzp == 0)
		handleMySqlError("fatal error initializing crossengine client lib", -1);
	try
	{
		drizzle_con_st* drzcp=0;
		drzcp = drizzle_con_add_tcp(drzp, fHost.c_str(), fPort, fUser.c_str(), fPasswd.c_str(),
			fSchema.c_str(), DRIZZLE_CON_MYSQL);
		if (drzcp == 0)
			handleMySqlError("fatal error setting up parms in crossengine client lib", -1);
		drizzle_return_t drzret;
		drzret = drizzle_con_connect(drzcp);
		if (drzret != 0)
			handleMySqlError("fatal error connecting to InfiniDB in crossengine client lib", drzret);

		string query(makeQuery());
		fLogger->logMessage(logging::LOG_TYPE_INFO, "QUERY to foreign engine: " + query);
		if (traceOn())
			cout << "QUERY: " << query << endl;

		drizzle_return_t* drzretp = &drzret;
		drizzle_result_st* drzrp=0;
		drzrp = drizzle_query_str(drzcp, drzrp, query.c_str(), drzretp);
		if (drzret != 0 || drzrp == 0)
			handleMySqlError("fatal error executing query in crossengine client lib", drzret);

		drzret = drizzle_result_buffer(drzrp);
		if (drzret != 0)
			handleMySqlError("fatal error reading results from InfiniDB in crossengine client lib", drzret);

		int num_fields = drizzle_result_column_count(drzrp);

		drizzle_row_t rowIn;                            // input
		shared_array<uint8_t> rgDataDelivered;      // output
		fRowGroupDelivered.initRow(&fRowDelivered);
		// use getDataSize() i/o getMaxDataSize() to make sure there are fRowsPerGroup rows.
		rgDataDelivered.reset(new uint8_t[fRowGroupDelivered.getDataSize(fRowsPerGroup)]);
		fRowGroupDelivered.setData(rgDataDelivered.get());
		fRowGroupDelivered.resetRowGroup(0);
		fRowGroupDelivered.getRow(0, &fRowDelivered);

		if (traceOn())
			dlTimes.setFirstReadTime();

		// Any functions to evaluate
		makeMappings();
		if (fFeSelects.empty() && fFeFilters == NULL)
		{
			while ((rowIn = drizzle_row_next(drzrp)) && fInputJobStepAssociation.status() == 0)
			{
				for(int i = 0; i < num_fields; i++)
					setField(i, rowIn[i], fRowDelivered);

				addRow(rgDataDelivered);
			}
		}

		else if (fFeSelects.empty() && fFeFilters != NULL)  // FE in WHERE clause only
		{
			shared_array<uint8_t> rgDataFe1;  // functions in where clause
			Row rowFe1;                       // row for fe evaluation
			fRowGroupFe1.initRow(&rowFe1);
			rgDataFe1.reset(new uint8_t[rowFe1.getSize()]);
			rowFe1.setData(rgDataFe1.get());
		
			while ((rowIn = drizzle_row_next(drzrp)) && fInputJobStepAssociation.status() == 0)
			{
				// Parse the columns used in FE1 first, the other column may not need be parsed.
				for(int i = 0; i < num_fields; i++)
				{
					if (fFe1Column[i] != -1)
						setField(fFe1Column[i], rowIn[i], rowFe1);
				}

				if (fFeInstance->evaluate(rowFe1, fFeFilters.get()) == false)
					continue;

				// Pass throug the parsed columns, and parse the remaining columns.
				applyMapping(fFeMapping1, rowFe1, &fRowDelivered);
				for(int i = 0; i < num_fields; i++)
				{
					if (fFe1Column[i] == -1)
						setField(i, rowIn[i], fRowDelivered);
				}

				addRow(rgDataDelivered);
			}
		}

		else if (!fFeSelects.empty() && fFeFilters == NULL)  // FE in SELECT clause only
		{
			shared_array<uint8_t> rgDataFe3;  // functions in select clause
			Row rowFe3;                       // row for fe evaluation
			fRowGroupOut.initRow(&rowFe3);
			rgDataFe3.reset(new uint8_t[rowFe3.getSize()]);
			rowFe3.setData(rgDataFe3.get());
		
			while ((rowIn = drizzle_row_next(drzrp)) && fInputJobStepAssociation.status() == 0)
			{
				for(int i = 0; i < num_fields; i++)
					setField(i, rowIn[i], rowFe3);

				fFeInstance->evaluate(rowFe3, fFeSelects);
				applyMapping(fFeMapping3, rowFe3, &fRowDelivered);

				addRow(rgDataDelivered);
			}
		}

		else  // FE in SELECT clause and WHERE clause
		{
			shared_array<uint8_t> rgDataFe1;  // functions in where clause
			Row rowFe1;                       // row for fe1 evaluation
			fRowGroupFe1.initRow(&rowFe1);
			rgDataFe1.reset(new uint8_t[rowFe1.getSize()]);
			rowFe1.setData(rgDataFe1.get());

			shared_array<uint8_t> rgDataFe3;  // functions in select clause
			Row rowFe3;                       // row for fe3 evaluation
			fRowGroupOut.initRow(&rowFe3);
			rgDataFe3.reset(new uint8_t[rowFe3.getSize()]);
			rowFe3.setData(rgDataFe3.get());
		
			while ((rowIn = drizzle_row_next(drzrp)) && fInputJobStepAssociation.status() == 0)
			{
				// Parse the columns used in FE1 first, the other column may not need be parsed.
				for(int i = 0; i < num_fields; i++)
				{
					if (fFe1Column[i] != -1)
						setField(fFe1Column[i], rowIn[i], rowFe1);
				}

				if (fFeInstance->evaluate(rowFe1, fFeFilters.get()) == false)
					continue;

				// Pass throug the parsed columns, and parse the remaining columns.
				applyMapping(fFeMapping1, rowFe1, &rowFe3);
				for(int i = 0; i < num_fields; i++)
				{
					if (fFe1Column[i] == -1)
						setField(i, rowIn[i], rowFe3);
				}

				fFeInstance->evaluate(rowFe3, fFeSelects);
				applyMapping(fFeMapping3, rowFe3, &fRowDelivered);

				addRow(rgDataDelivered);
			}
		}

		fOutputDL->insert(rgDataDelivered);
		fRowsRetrieved = drizzle_result_row_count(drzrp);
		drizzle_result_free(drzrp);
		drzrp = 0;
		drizzle_con_close(drzcp);
		drizzle_con_free(drzcp);
		drzcp = 0;
		drizzle_free(drzp);
		drzp = 0;
	}
	catch (IDBExcept& iex)
	{
		catchHandler(iex.what(), fSessionId);
		if (fOutputJobStepAssociation.status() == 0)
		{
			fOutputJobStepAssociation.status(iex.errorCode());
			fOutputJobStepAssociation.errorMessage(iex.what());
		}
	}
	catch(const std::exception& ex)
	{
		catchHandler(ex.what(), fSessionId);
		if (fOutputJobStepAssociation.status() == 0)
		{
			fOutputJobStepAssociation.status(ERR_CROSS_ENGINE_CONNECT);
			fOutputJobStepAssociation.errorMessage(ex.what());
		}
	}
	catch(...)
	{
		catchHandler("CrossEngineStep execute caught an unknown exception", fSessionId);
		if (fOutputJobStepAssociation.status() == 0)
		{
			fOutputJobStepAssociation.status(ERR_CROSS_ENGINE_CONNECT);
			fOutputJobStepAssociation.errorMessage("unknown exception");
		}
	}

	fEndOfResult = true;
	fOutputDL->endOfInput();

	// Bug 3136, let mini stats to be formatted if traceOn.
	if (traceOn())
	{
		dlTimes.setLastReadTime();
		dlTimes.setEndOfInputTime();
		printCalTrace();
	}
}


void CrossEngineStep::setBPP(JobStep* jobStep)
{
	pColStep* pcs = dynamic_cast<pColStep*>(jobStep);
	pColScanStep* pcss = NULL;
	pDictionaryStep* pds = NULL;
	pDictionaryScan* pdss = NULL;
	FilterStep* fs = NULL;
	string bop = " AND ";
	if (pcs != 0)
	{
		if (pcs->BOP() == BOP_OR)
			bop = " OR ";
		addFilterStr(pcs->getFilters(), bop);
	}
	else if ((pcss = dynamic_cast<pColScanStep*>(jobStep)) != 0)
	{
		if (pcss->BOP() == BOP_OR)
			bop = " OR ";
		addFilterStr(pcss->getFilters(), bop);
	}
	else if ((pds = dynamic_cast<pDictionaryStep*>(jobStep)) != 0)
	{
		if (pds->BOP() == BOP_OR)
			bop = " OR ";
		addFilterStr(pds->getFilters(), bop);
	}
	else if ((pdss = dynamic_cast<pDictionaryScan*>(jobStep)) != 0)
	{
		if (pds->BOP() == BOP_OR)
			bop = " OR ";
		addFilterStr(pdss->getFilters(), bop);
	}
	else if ((fs = dynamic_cast<FilterStep*>(jobStep)) != 0)
	{
		addFilterStr(fs->getFilters(), bop);
	}
}

void CrossEngineStep::addFilterStr(const vector<const Filter*>& f, const string& bop)
{
	if (f.size() == 0)
		return;

	string filterStr;
	for (uint64_t i = 0; i < f.size(); i++)
	{
		if (f[i]->data().empty())
			continue;

		if (!filterStr.empty())
			filterStr += bop;

		filterStr += f[i]->data();
	}

	if (!filterStr.empty())
	{
		if (!fWhereClause.empty())
			fWhereClause += " AND (" + filterStr + ")";
		else
			fWhereClause += " WHERE (" + filterStr + ")";
	}
}


void CrossEngineStep::setProjectBPP(JobStep* jobStep1, JobStep*)
{
	fColumnMap[jobStep1->tupleId()] = fColumnCount++;

	if (!fSelectClause.empty())
		fSelectClause += ", ";
	else
		fSelectClause += "SELECT ";

	fSelectClause += jobStep1->name();
}


string CrossEngineStep::makeQuery()
{
	ostringstream oss;
	oss << fSelectClause << " FROM " << fTable;
	if (fTable.compare(fAlias) != 0)
		oss << " " << fAlias;

	if (!fWhereClause.empty())
		oss << fWhereClause;

	// the string must consist of a single SQL statement without a terminating semicolon ; or \g.
	// oss << ";";

	return oss.str();
}

void CrossEngineStep::handleMySqlError(const char* errStr, unsigned int errCode)
{
	ostringstream oss;
	oss << errStr << "(" << errCode << ")";
	throw IDBExcept(oss.str(), ERR_CROSS_ENGINE_CONNECT);

	return;
}


const RowGroup& CrossEngineStep::getOutputRowGroup() const
{
	return fRowGroupOut;
}


const RowGroup& CrossEngineStep::getDeliveredRowGroup() const
{
	return fRowGroupDelivered;
}


uint CrossEngineStep::nextBand(messageqcpp::ByteStream &bs)
{
	shared_array<uint8_t> rgDataOut;
	bool more = false;
	uint rowCount = 0;

	try
	{
		bs.restart();

		more = fOutputDL->next(fOutputIterator, &rgDataOut);
		if (traceOn() && dlTimes.FirstReadTime().tv_sec ==0)
			dlTimes.setFirstReadTime();

		if (more && (0 == fOutputJobStepAssociation.status()) && !die)
		{
			fRowGroupDelivered.setData(rgDataOut.get());
			bs.load(fRowGroupDelivered.getData(), fRowGroupDelivered.getDataSize());
			rowCount = fRowGroupDelivered.getRowCount();
		}
		else
		{
			if (more) more = fOutputDL->next(fOutputIterator, &rgDataOut);
			fEndOfResult = true;
		}
	}
	catch(const std::exception& ex)
	{
		catchHandler(ex.what(), fSessionId);
		if (fOutputJobStepAssociation.status() == 0)
			fOutputJobStepAssociation.status(ERR_IN_DELIVERY);
		while (more) more = fOutputDL->next(fOutputIterator, &rgDataOut);
		fEndOfResult = true;
	}
	catch(...)
	{
		catchHandler("CrossEngineStep next band caught an unknown exception", fSessionId);
		if (fOutputJobStepAssociation.status() == 0)
			fOutputJobStepAssociation.status(ERR_IN_DELIVERY);
		while (more) more = fOutputDL->next(fOutputIterator, &rgDataOut);
		fEndOfResult = true;
	}

	if (fEndOfResult)
	{
		// send an empty / error band
		rgDataOut.reset(new uint8_t[fRowGroupDelivered.getEmptySize()]);
		fRowGroupDelivered.setData(rgDataOut.get());
		fRowGroupDelivered.resetRowGroup(0);
		fRowGroupDelivered.setStatus(fOutputJobStepAssociation.status());
		bs.load(rgDataOut.get(), fRowGroupDelivered.getDataSize());

		if (traceOn())
		{
			dlTimes.setLastReadTime();
			dlTimes.setEndOfInputTime();
		}

		if (traceOn())
			printCalTrace();
	}

	return rowCount;
}


const string CrossEngineStep::toString() const
{
	ostringstream oss;
	oss << "CrossEngineStep ses:" << fSessionId << " txn:" << fTxnId << " st:" << fStepId;

	oss << " in:";
	for (unsigned i = 0; i < fInputJobStepAssociation.outSize(); i++)
		oss << fInputJobStepAssociation.outAt(i);

	oss << " out:";
	for (unsigned i = 0; i < fOutputJobStepAssociation.outSize(); i++)
		oss << fOutputJobStepAssociation.outAt(i);

	oss << endl;

	return oss.str();
}


void CrossEngineStep::printCalTrace()
{
	time_t t = time (0);
	char timeString[50];
	ctime_r (&t, timeString);
	timeString[strlen (timeString )-1] = '\0';
	ostringstream logStr;
	logStr  << "ses:" << fSessionId << " st: " << fStepId << " finished at "<< timeString
			<< "; rows retrieved-" << fRowsRetrieved
			<< "; total rows returned-" << fRowsReturned << endl
			<< "\t1st read " << dlTimes.FirstReadTimeString()
			<< "; EOI " << dlTimes.EndOfInputTimeString() << "; runtime-"
			<< JSTimeStamp::tsdiffstr(dlTimes.EndOfInputTime(), dlTimes.FirstReadTime())
			<< "s;\n\tJob completion status " << fOutputJobStepAssociation.status() << endl;
	logEnd(logStr.str().c_str());
	fExtendedInfo += logStr.str();
	formatMiniStats();
}


void CrossEngineStep::formatMiniStats()
{
	ostringstream oss;
	oss << "CES "
		<< "UM "
		<< "- "
		<< "- "
		<< "- "
		<< "- "
		<< "- "
		<< "- "
		<< JSTimeStamp::tsdiffstr(dlTimes.EndOfInputTime(), dlTimes.FirstReadTime()) << " "
		<< fRowsReturned << " ";
	fMiniInfo += oss.str();
}


// static
void CrossEngineStep::init_mysqlcl_idb()
{
}


}   //namespace
// vim:ts=4 sw=4:


