/*

   Copyright (C) 2009-2012 Calpont Corporation.

   Use of and access to the Calpont InfiniDB Community software is subject to the
   terms and conditions of the Calpont Open Source License Agreement. Use of and
   access to the Calpont InfiniDB Enterprise software is subject to the terms and
   conditions of the Calpont End User License Agreement.

   This program is distributed in the hope that it will be useful, and unless
   otherwise noted on your license agreement, WITHOUT ANY WARRANTY; without even
   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   Please refer to the Calpont Open Source License Agreement and the Calpont End
   User License Agreement for more details.

   You should have received a copy of either the Calpont Open Source License
   Agreement or the Calpont End User License Agreement along with this program; if
   not, it is your responsibility to review the terms and conditions of the proper
   Calpont license agreement by visiting http://www.calpont.com for the Calpont
   InfiniDB Enterprise End User License Agreement or http://www.infinidb.org for
   the Calpont InfiniDB Community Calpont Open Source License Agreement.

   Calpont may make changes to these license agreements from time to time. When
   these changes are made, Calpont will make a new copy of the Calpont End User
   License Agreement available at http://www.calpont.com and a new copy of the
   Calpont Open Source License Agreement available at http:///www.infinidb.org.
   You understand and agree that if you use the Program after the date on which
   the license agreement authorizing your use has changed, Calpont will treat your
   use as acceptance of the updated License.

*/

/*******************************************************************************
* $Id$
*
*******************************************************************************/

/*
 * we_filereadthread.cpp
 *
 *  Created on: Oct 25, 2011
 *      Author: bpaul
 */


#include "we_messages.h"
#include "we_sdhandler.h"

#include <boost/thread/condition.hpp>
#include <boost/scoped_array.hpp>
#include <boost/thread.hpp>
using namespace boost;

#include "messagequeue.h"
#include "bytestream.h"
using namespace messageqcpp;

#include <fstream>
#include <istream>
#include <list>
using namespace std;

#include "we_filereadthread.h"

namespace WriteEngine
{

void WEReadThreadRunner::operator ()()
{
	try
	{
		fRef.feedData();
	}
	catch(std::exception& ex)
	{
		throw runtime_error(ex.what());
	}
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------



WEFileReadThread::WEFileReadThread(WESDHandler& aSdh):fSdh(aSdh),
		fpThread(0),
		fFileMutex(),
		fContinue(true),
		fInFileName(),
		fInFile(std::cin.rdbuf()), //@BUG 4326
		fTgtPmId(0),
		fBatchQty(0),
		fEnclEsc(false),
		fEncl('\0'),
		fEsc('\0'),
		fDelim('|')
{
	//TODO batch qty to get from config
	fBatchQty = 10000;

}

//WEFileReadThread::WEFileReadThread(const WEFileReadThread& rhs):fSdh(rhs.fSdh)
//{
//	// TODO copy constructor
//}


WEFileReadThread::~WEFileReadThread()
{
	//if(fInFile.is_open()) fInFile.close(); //@BUG 4326
	if(fIfFile.is_open()) fIfFile.close();
	setTgtPmId(0);
	if (fpThread)
	{
		delete fpThread;
	}
	fpThread=0;
	//cout << "WEFileReadThread destructor called" << endl;
}

//------------------------------------------------------------------------------

void WEFileReadThread::reset()
{
	//if(fInFile.is_open()) fInFile.close(); //@BUG 4326
	if(fIfFile.is_open()) fIfFile.close();
	setTgtPmId(0);
	if (fpThread)
	{
		delete fpThread;
	}
	fpThread=0;
	//cout << "WEFileReadThread destructor called" << endl;
	this->setContinue(true);
}
//------------------------------------------------------------------------------

void WEFileReadThread::setup(std::string FileName)
{
	if(fSdh.getDebugLvl()) 
		cout << "WEFileReadThread::setup : *** Input files = " << FileName <<endl;

	reset();

	try
	{
		char aEncl = fSdh.getEnclChar();
		char aEsc = fSdh.getEscChar();
		char aDelim = fSdh.getDelimChar();

		if(aEncl) fEncl = aEncl;
		if(aEsc) fEsc = aEsc;
		if(aDelim) fDelim = aDelim;

		if((aEncl != 0)||(aEsc != 0)) fEnclEsc = true;

		//BUG 4342 - Need to support "list of infiles"
		//chkForListOfFiles(FileName); - List prepared in sdhandler.

		string aStrName = getNextInputDataFile();

		if(fSdh.getDebugLvl()>2)
			cout << "Next InFileName = " << aStrName << endl;

		setInFileName(aStrName);
		//setInFileName(FileName);
		openInFile();
		//set the target PM
		fpThread = new boost::thread(WEReadThreadRunner(*this));
	}
	catch(std::exception& ex)
	{
		//cout << ex.what() << endl;
		//throw runtime_error("Exception occured in WEFileReadThread\n");
		throw runtime_error(ex.what());
	}

	if(fpThread)
	{
		// Need to send a all clear??
	}
}

//------------------------------------------------------------------------------

bool WEFileReadThread::chkForListOfFiles(std::string& FileName)
{
	//cout << "Inside chkForListOfFiles("<< FileName << ")" << endl;
	std::string aFileName = FileName;

	istringstream iss(aFileName);
	size_t start = 0, end = 0;
	const char* sep = " ,|";

	end = aFileName.find_first_of(sep);
	do
	{
		if(end != string::npos)
		{
			std::string aFile = aFileName.substr(start, end-start);
			if(fSdh.getDebugLvl()>2)
				cout << "File: " << aFileName.substr(start, end-start) << endl;
			start = end + 1;
			fInfileList.push_back(aFile);
		}
		else
		{
			std::string aFile = aFileName.substr(start, end-start);
			if(fSdh.getDebugLvl()>1)
				cout << "Next Input File " << aFileName.substr(start, end-start) << endl;
			fInfileList.push_back(aFile);
			break;
		}
		end = aFileName.find_first_of(sep, start);
	}
	while(start != end);

	//cout << "Going out chkForListOfFiles("<< FileName << ")" << endl;

	return false;
}
//------------------------------------------------------------------------------

std::string WEFileReadThread::getNextInputDataFile()
{
	std::string aNextFile;

	if(fInfileList.size()>0)
	{
		aNextFile = fInfileList.front();
		fInfileList.pop_front();
	}

	//cout << "Next Input DataFile = " << aNextFile << endl;

	return aNextFile;
}
//------------------------------------------------------------------------------

void WEFileReadThread::add2InputDataFileList(std::string& FileName)
{
	fInfileList.push_front(FileName);
}
//------------------------------------------------------------------------------

void WEFileReadThread::shutdown()
{
	this->setContinue(false);
	mutex::scoped_lock aLock(fFileMutex);	//wait till readDataFile() finish
	//if(fInFile.is_open()) fInFile.close(); //@BUG 4326
	if(fIfFile.is_open()) fIfFile.close();
}

//------------------------------------------------------------------------------

void WEFileReadThread::feedData()
{
	unsigned int aRowCnt = 0;
	const unsigned int c10mSec = 10000;
	while(isContinue())
	{
		unsigned int TgtPmId = getTgtPmId();
		if(TgtPmId == 0)
		{
			setTgtPmId(fSdh.getNextPm2Feed());
			TgtPmId = getTgtPmId();
		}

		if((TgtPmId>0)&&(fInFile.good()))
		{
			try
			{
				messageqcpp::SBS aSbs(new messageqcpp::ByteStream);
				aRowCnt = readDataFile(aSbs);
				//cout << "Length " << aSbs->length() <<endl;    - for debug
				fSdh.updateRowTx(aRowCnt, TgtPmId);
				mutex::scoped_lock aLock(fSdh.fSendMutex);
				fSdh.send2Pm(aSbs, TgtPmId);
				aLock.unlock();
				setTgtPmId(0);	//reset PmId. Send the data to next least data
			}
			catch(std::exception& ex)
			{
				throw runtime_error(ex.what());
			}
		}
		else
		{
			usleep(c10mSec);
			setTgtPmId(0);
		}

		// Finish reading file and thread can go away once data sent
		if(fInFile.eof())
		{
			if(fInfileList.size() != 0)
			{
				if(fIfFile.is_open()) fIfFile.close();
				string aStrName = getNextInputDataFile();
				setInFileName(aStrName);
				openInFile();
			}
			else
			{
				//if there is no more files to be read send EOD
				//cout << "Sending EOD message to PM" << endl;
				fSdh.sendEODMsg();
				setContinue(false);
			}
		}

	}
}


//------------------------------------------------------------------------------

unsigned int WEFileReadThread::readDataFile(messageqcpp::SBS& Sbs)
{
	mutex::scoped_lock aLock(fFileMutex);
	// For now we are going to send KEEPALIVES
	//*Sbs << (ByteStream::byte)(WE_CLT_SRV_KEEPALIVE);
	if((fInFile.good()) && (!fInFile.eof()))
	{

		//cout << "Inside WEFileReadThread::readDataFile" << endl;
		//char aBuff[1024*1024];			// TODO May have to change it later
		//char*pStart = aBuff;
		unsigned int aIdx=0;
		unsigned int aLen=0;
		*Sbs << (ByteStream::byte)(WE_CLT_SRV_DATA);
		while((!fInFile.eof())&&(aIdx< getBatchQty()))
		{
			if(fEnclEsc)
			{
				//pStart = aBuff;
				aLen = getNextRow(fInFile, fBuff, sizeof(fBuff)-1);
			}
			else
			{
				fInFile.getline(fBuff, sizeof(fBuff)-1);
				aLen=fInFile.gcount();
			}
			////aLen chars incl \n, Therefore aLen-1; '<<' oper won't go past it
			//cout << "Data Length " << aLen <<endl;
			if((aLen < (sizeof(fBuff)-2)) && (aLen>0))
			{
				fBuff[aLen-1] = '\n';
				fBuff[aLen]=0;
				if(fSdh.getDebugLvl()>3) cout << "Data Read " << fBuff <<endl;
				(*Sbs).append(reinterpret_cast<ByteStream::byte*>(fBuff), aLen);
				aIdx++;
				if(fSdh.getDebugLvl()>2) cout << "File data line = " << aIdx <<endl;
			}
			else if(aLen>=sizeof(fBuff)-2)	//Didn't hit delim; BIG ROW
			{
				cout <<"Bad Row data " << endl;
				cout << fBuff << endl;
				throw runtime_error("Data Row too BIG to handle!!");
			}
			//for debug
			//if(fSdh.getDebugLvl()>3) cout << aIdx << endl;
		}// while
		return aIdx;
	}// if
	return 0;
}
//------------------------------------------------------------------------------

void WEFileReadThread::openInFile()
{
	try
	{
		if(fSdh.getDebugLvl()) cout << "Input FileName: " << fInFileName <<endl;
		if(fInFileName == "/dev/stdin")
		{
			char aDefCon[16], aGreenCol[16];
			snprintf(aDefCon, sizeof(aDefCon), "\033[0m");
			snprintf(aGreenCol, sizeof(aGreenCol), "\033[0;32m");
			if(fSdh.getDebugLvl())			// BUG 4195
				cout << aGreenCol
					 << "trying to read from STDIN... "
					 << aDefCon << endl;
		}
		cout.flush();

		//@BUG 4326
		if(fInFileName != "/dev/stdin")
		{
			if (!fIfFile.is_open()) fIfFile.open(fInFileName.c_str());
			if (!fIfFile.good())
				throw runtime_error("Could not open Input file "+fInFileName);
			fInFile.rdbuf(fIfFile.rdbuf()); //@BUG 4326
		}
		//@BUG 4326  -below three lines commented out
		//		if (!fInFile.is_open()) fInFile.open(fInFileName.c_str());
		//		if (!fInFile.good())
		//			throw runtime_error("Could not open Input file "+fInFileName);

	}
	catch(std::exception& ex)
	{
		cout << "Error in Opening input data file "<< fInFileName << endl;
		throw runtime_error(ex.what());	//BUG 4201 FIX
	}
}

//------------------------------------------------------------------------------

int WEFileReadThread::getNextRow(istream& ifs, char* pBuf, int MaxLen)
{
	//const char ENCL ='\"';		//TODO for time being
	//const char ESC = '\0';		//TODO for time being
	const char ENCL = fEncl;
	const char ESC = fEsc;
	bool aTrailEsc = false;
	char *pEnd = pBuf;
	char aCh = ifs.get();
	while (ifs.good())
	{
		if(aCh == ENCL)
		{
			// we got the first enclosedBy char.
			*pEnd++ = aCh;
			aCh = ifs.get();
			//cout << "aCh 1 = " << aCh << endl;
			while(aCh != ENCL)		//Loop thru till we hit another one
			{
				if(aCh == ESC) //check spl cond ESC inside ENCL of '\n' here
				{
					*pEnd++ = aCh;
					aCh = ifs.get();
					*pEnd++ = aCh;
					aCh = ifs.get();	// get the next char for while loop
					//cout << "aCh 2 = " << aCh << endl;
				}// case ESC
				else
				{
					*pEnd++ = aCh;
					aCh = ifs.get();
					//cout << "aCh 3 = " << aCh << endl;
				}
			}
			*pEnd++ = aCh;	// ENCL char got
			aTrailEsc = true;	//@BUG 4641
		}//case ENCL
		else if(aCh == ESC)
		{
			*pEnd++ = aCh;
			aCh = ifs.get();
			*pEnd++ = aCh;
			//cout << "aCh 4 = " << aCh << endl;
		}// case ESC
		else
		{
			*pEnd++ = aCh;
			//cout << "aCh 5 = " << aCh << endl;
		}
		//cout << "pBuf1 " << pBuf << endl;
		if((aCh == '\n')||((pEnd-pBuf)==MaxLen)) break;	// we got a full row
		aCh = ifs.get();

		// BUG 4641 To avoid seg fault when a wrong/no ESC char provided.
		while(aTrailEsc)
		{
			// BUG 4903  EOF, to handle files ending w/ EOF and w/o '\n'
			if((aCh == '\n')||(aCh == EOF) ||(aCh == fDelim))
			{
				aTrailEsc = false;
				break;
			}
			else
			{
				*pEnd++ = aCh;
				aCh = ifs.get();
			}
		}

	}// end of while loop

	return pEnd - pBuf;
}

//------------------------------------------------------------------------------


} /* namespace WriteEngine */

