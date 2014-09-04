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
 * Observer.cpp
 *
 *  Created on: Oct 5, 2011
 *      Author: bpaul@calpont.com
 */

#include <boost/thread/mutex.hpp>

#include "we_observer.h"


namespace WriteEngine {

//-----------------------------------------------------------------------------

//	ctor
Observer::Observer() {

}

//-----------------------------------------------------------------------------
//dtor
Observer::~Observer() {
	//
}

//-----------------------------------------------------------------------------
//ctor
Subject::Subject(){

}
//-----------------------------------------------------------------------------
//dtor
Subject::~Subject(){

}

//-----------------------------------------------------------------------------

void Subject::attach(Observer* Obs) {
	boost::mutex::scoped_lock aLstLock;
	fObs.push_back(Obs);
}

//-----------------------------------------------------------------------------

void Subject::detach(Observer* Obs) {
	boost::mutex::scoped_lock aLstLock;
	Observers::iterator aIt = fObs.begin();
	while (aIt != fObs.end()) {
		if ((*aIt) == Obs) {
			fObs.erase(aIt);
			break;
		}
	}
}

//-----------------------------------------------------------------------------

void Subject::notify() {
	boost::mutex::scoped_lock aLstLock;
	Observers::iterator aIt = fObs.begin();
	while (aIt != fObs.end()) {
		(*aIt)->update(this);
	}
}

//-----------------------------------------------------------------------------

}// namespace WriteEngine

