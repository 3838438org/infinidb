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
* $Id: func_trim.cpp 3741 2013-04-25 20:36:44Z bpaul $
*
*
****************************************************************************/

#include <string>
using namespace std;

#include "functor_str.h"
#include "functioncolumn.h"
#include "utils_utf8.h"
using namespace execplan;

#include "rowgroup.h"
using namespace rowgroup;

#include "joblisttypes.h"
using namespace joblist;

namespace funcexp
{
CalpontSystemCatalog::ColType Func_trim::operationType(FunctionParm& fp, CalpontSystemCatalog::ColType& resultType)
{
	// operation type is not used by this functor
	return fp[0]->data()->resultType();
}


std::string Func_trim::getStrVal(rowgroup::Row& row,
						FunctionParm& fp,
						bool& isNull,
						execplan::CalpontSystemCatalog::ColType&)
{
    // The number of characters (not bytes) in our input tstr.
    // Not all of these are necessarily significant. We need to search for the 
    // NULL terminator to be sure.
    size_t strwclen; 
    // this holds the number of characters (not bytes) in ourtrim tstr.
    size_t trimwclen;

	// The original string
	string tstr = fp[0]->data()->getStrVal(row, isNull);

	// The trim characters.
	string trim = (fp.size() > 1 ? fp[1]->data()->getStrVal(row, isNull) : " ");
    
	
	if (isNull)
        return "";
    if (tstr.empty() || tstr.length() == 0)
        return tstr;

    // Rather than calling the wideconvert functions with a null buffer to 
    // determine the size of buffer to allocate, we can be sure the wide
    // char string won't be longer than:
    strwclen = tstr.length(); // a guess to start with. This will be >= to the real count.
    int bufsize = (strwclen+1) * sizeof(wchar_t);

    // Convert the string to wide characters. Do all further work in wide characters
    wchar_t* wcbuf = (wchar_t*)alloca(bufsize);
    strwclen = utf8::idb_mbstowcs(wcbuf, tstr.c_str(), strwclen+1);
	// Bad char in mbc can return -1
	if(strwclen == static_cast<size_t>(-1))
		strwclen = 0;

    // Convert the trim string to wide
    trimwclen = trim.length();  // A guess to start.
    int trimbufsize = (trimwclen+1) * sizeof(wchar_t);
    wchar_t* wctrim = (wchar_t*)alloca(trimbufsize);
    size_t trimlen = utf8::idb_mbstowcs(wctrim,trim.c_str(), trimwclen+1);
	// Bad char in mbc can return -1
	if(trimlen == static_cast<size_t>(-1))
		trimlen = 0;
    size_t trimCmpLen = trimlen * sizeof(wchar_t);

    const wchar_t* oPtr = wcbuf;      // To remember the start of the string
    const wchar_t* aPtr = oPtr;
    const wchar_t* aEnd = wcbuf+strwclen-1;
    size_t trimCnt = 0;

	if(trimlen > 0)
	{
		if (trimlen == 1)
		{
			// If trim is a single char, then don't spend the overhead for memcmp.
			wchar_t chr = wctrim[0];
			// remove leading
			while (aPtr != aEnd && *aPtr == chr)
			{
				aPtr++;
				++trimCnt;
			}
			// remove trailing
			while (aEnd != aPtr && *aEnd == chr)
			{
				aEnd--;
				++trimCnt;
			}
		}
		else
		{
			aEnd-=(trimlen-1);	// So we don't compare past the end of the string.
			// remove leading
			while (aPtr <= aEnd && !memcmp(aPtr, wctrim, trimCmpLen))
			{
				aPtr+=trimlen;
				trimCnt+=trimlen;
			}
			// remove trailing
			while (aPtr <= aEnd && !memcmp(aEnd, wctrim, trimCmpLen))
			{
				aEnd-=trimlen;	//BUG 5241
				trimCnt+=trimlen;
			}
		}
	}
	// Bug 5110 - error in allocating enough memory for utf8 chars 
	size_t aLen = strwclen-trimCnt;
	wstring trimmed = wstring(aPtr, aLen);
	// Turn back to a string
	return utf8::wstring_to_utf8(trimmed.c_str());
}							


} // namespace funcexp
// vim:ts=4 sw=4:

