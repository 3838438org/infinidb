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

/****************************************************************************
* $Id: func_ceil.cpp 3048 2012-04-04 15:33:45Z rdempsey $
*
*
****************************************************************************/

#include <cstdlib>
#include <iomanip>
#include <string>
using namespace std;

#include "functor_real.h"
#include "functioncolumn.h"
using namespace execplan;

#include "rowgroup.h"
using namespace rowgroup;

#include "errorcodes.h"
#include "idberrorinfo.h"
#include "errorids.h"
using namespace logging;
using namespace dataconvert;

#include "funchelpers.h"

namespace funcexp
{

CalpontSystemCatalog::ColType Func_ceil::operationType(FunctionParm& fp, CalpontSystemCatalog::ColType& resultType)
{
	return fp[0]->data()->resultType();
}


int64_t Func_ceil::getIntVal(Row& row,
							FunctionParm& parm,
							bool& isNull,
							CalpontSystemCatalog::ColType& op_ct)
{
	int64_t ret = 0;

	switch (op_ct.colDataType)
	{
		case CalpontSystemCatalog::BIGINT:
		case CalpontSystemCatalog::INT:
		case CalpontSystemCatalog::MEDINT:
		case CalpontSystemCatalog::TINYINT:
		case CalpontSystemCatalog::SMALLINT:
		case CalpontSystemCatalog::DECIMAL:
		{
			IDB_Decimal decimal = parm[0]->data()->getDecimalVal(row, isNull);

			if (isNull)
				break;

			ret = decimal.value;
			// negative scale is not supported by CNX yet
			if (decimal.scale > 0)
			{

				if (decimal.scale >= 19)
				{
					std::ostringstream oss;
					oss << "ceil: datatype of " << colDataTypeToString(op_ct.colDataType)
						<< " with scale " << (int) decimal.scale << " is beyond supported scale";
					throw logging::IDBExcept(oss.str(), ERR_DATATYPE_NOT_SUPPORT);
				}

				// Adjust to an int based on the scale.
				int64_t tmp = ret;
				ret /= powerOf10_c[decimal.scale];

				// Add 1 if this is a positive number and there were values to the right of the
				// decimal point so that we return the largest integer value not less than X.
				if ((tmp - (ret * powerOf10_c[decimal.scale]) > 0))
				ret += 1;
			}
		}
		break;

		case CalpontSystemCatalog::DOUBLE:
		case CalpontSystemCatalog::FLOAT:
		{
			ret = (int64_t) ceil(parm[0]->data()->getDoubleVal(row, isNull));
		}
		break;

		case CalpontSystemCatalog::VARCHAR:
		case CalpontSystemCatalog::CHAR:
		{
			string str = parm[0]->data()->getStrVal(row, isNull);
			if (!isNull)
				ret = (int64_t) ceil(strtod(str.c_str(), 0));
		}
		break;

		case CalpontSystemCatalog::DATE:
		{
			string str = DataConvert::dateToString1(parm[0]->data()->getDateIntVal(row, isNull));
			if (!isNull)
				ret = atoll(str.c_str());
		}
		break;

		case CalpontSystemCatalog::DATETIME:
		{
			string str =
				DataConvert::datetimeToString1(parm[0]->data()->getDatetimeIntVal(row, isNull));

			// strip off micro seconds
			str = str.substr(0,14);

			if (!isNull)
				ret = atoll(str.c_str());
		}
		break;

		default:
		{
			std::ostringstream oss;
			oss << "ceil: datatype of " << colDataTypeToString(op_ct.colDataType)
				<< " is not supported";
			throw logging::IDBExcept(oss.str(), ERR_DATATYPE_NOT_SUPPORT);
		}
	}
	return ret;
}


double Func_ceil::getDoubleVal(Row& row,
							FunctionParm& parm,
							bool& isNull,
							CalpontSystemCatalog::ColType& op_ct)
{
	double ret = 0.0;
	if (op_ct.colDataType == CalpontSystemCatalog::DOUBLE ||
		op_ct.colDataType == CalpontSystemCatalog::FLOAT)
	{
		ret = ceil(parm[0]->data()->getDoubleVal(row, isNull));
	}
	else if (op_ct.colDataType == CalpontSystemCatalog::VARCHAR ||
			 op_ct.colDataType == CalpontSystemCatalog::CHAR)
	{
		string str = parm[0]->data()->getStrVal(row, isNull);
		if (!isNull)
			ret = ceil(strtod(str.c_str(), 0));
	}
	else
	{
		ret = (double) getIntVal(row, parm, isNull, op_ct);
	}

	return ret;
}


string Func_ceil::getStrVal(Row& row,
							FunctionParm& parm,
							bool& isNull,
							CalpontSystemCatalog::ColType& op_ct)
{
	string ret;
	if (op_ct.colDataType == CalpontSystemCatalog::DOUBLE ||
		op_ct.colDataType == CalpontSystemCatalog::FLOAT ||
		op_ct.colDataType == CalpontSystemCatalog::VARCHAR ||
		op_ct.colDataType == CalpontSystemCatalog::CHAR)
	{
		ostringstream oss;
		oss << fixed << getDoubleVal(row, parm, isNull, op_ct);

		// remove the decimals in the oss string.
		ret = oss.str();
		size_t d = ret.find('.');
		ret = ret.substr(0, d);
	}
	else
	{
		ostringstream oss;
		oss << getIntVal(row, parm, isNull, op_ct);
		ret = oss.str();
	}

	return ret;
}


} // namespace funcexp
// vim:ts=4 sw=4:
