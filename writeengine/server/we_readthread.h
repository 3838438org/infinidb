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

/*******************************************************************************
* $Id: we_readthread.h 4216 2012-09-28 23:08:21Z xlou $
*
*******************************************************************************/
#ifndef WE_SRV_READTHREAD_H__
#define WE_SRV_READTHREAD_H__

#include <unistd.h>
#include <boost/shared_ptr.hpp>
#include "messagequeue.h"
#include "threadpool.h"
#include "we_ddlcommandproc.h"
using namespace threadpool;

#include "we_ddlcommandproc.h"
#include "we_dmlcommandproc.h"
#include "we_cleartablelockcmd.h"
#include "we_dataloader.h"



namespace WriteEngine
{


class ReadThread
{
public:
	explicit ReadThread(const messageqcpp::IOSocket& ios);
	virtual ~ReadThread();

	virtual void operator()();


//protected:
public:
	messageqcpp::IOSocket fIos;
    messageqcpp::ByteStream fIbs;

private:
	//defaults okay
	//ReadThread(const ReadThread& rhs);
	//ReadThread& operator=(const ReadThread& rhs);

};


class DdlReadThread: public ReadThread
{
public:
	explicit DdlReadThread(const messageqcpp::IOSocket& ios, ByteStream& ibs );
	virtual ~DdlReadThread();

	virtual void operator()();


private:
	boost::shared_ptr<WE_DDLCommandProc> fWeDDLprocessor;


};

class DmlReadThread: public ReadThread
{
public:
	explicit DmlReadThread(const messageqcpp::IOSocket& ios, ByteStream& ibs );
	virtual ~DmlReadThread();

	virtual void operator()();


private:
	boost::shared_ptr<WE_DMLCommandProc> fWeDMLprocessor;
	boost::shared_ptr<WE_DDLCommandProc> fWeDDLprocessor;

};

class SplitterReadThread: public ReadThread
{
public:
	SplitterReadThread(const messageqcpp::IOSocket& ios, ByteStream& Ibs);
    SplitterReadThread(const SplitterReadThread& rhs);
	virtual ~SplitterReadThread();

	virtual void operator()();


private:
    //WEDataLoader* fpWeDataLoader;
    WEDataLoader fWeDataLoader;

friend class ReadThreadFactory; 
//friend class WEDataLoader;
};


//------------------------------------------------------------------------------
// Thread object that receives a cleartablelock tool command.
//------------------------------------------------------------------------------
class ClearTableLockReadThread : public ReadThread
{
public:
	ClearTableLockReadThread(const messageqcpp::IOSocket& ios, ByteStream& ibs);
	virtual ~ClearTableLockReadThread();

	virtual void operator()();
private:
	boost::shared_ptr<WE_ClearTableLockCmd> fWeClearTableLockCmd;
};


//------------------------------------------------------------------------------
// Thread object that receives a redistributeDB tool command.
//------------------------------------------------------------------------------
class RedistributeReadThread : public ReadThread
{
public:
	RedistributeReadThread(const messageqcpp::IOSocket& ios, ByteStream& ibs);
	virtual ~RedistributeReadThread();

	virtual void operator()();

private:
	
};


class ReadThreadFactory
{
public:
	ReadThreadFactory(){}
	virtual ~ReadThreadFactory(){}

public:
	static void CreateReadThread(ThreadPool& Tp, IOSocket& ios);


};


}

#endif

