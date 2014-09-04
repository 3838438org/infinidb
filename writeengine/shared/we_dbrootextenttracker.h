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

/*
* $Id: we_dbrootextenttracker.h 4195 2012-09-19 18:12:27Z dcathey $
*/

/** @file we_dbrootextenttracker.h
 * Contains classes to track the order of placement (rotation) of extents as
 * they are filled in and/or added to the DBRoots for the local PM.
 */

#ifndef WE_DBROOTEXTENTTRACKER_H_
#define WE_DBROOTEXTENTTRACKER_H_

#include <boost/thread/mutex.hpp>
#include <vector>

#include "we_type.h"
#include "brmtypes.h"

#if defined(_MSC_VER) && defined(WRITEENGINEDBEXTTRK_DLLEXPORT)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace WriteEngine
{
    class Log;

//
// PARTIAL_EXTENT - Extent is partially filled
// EMPTY_DBROOT   - DRoot is empty (has no extents)
// EXTENT_BOUNDARY- Encountered extent boundary, add next extent
//
enum DBRootExtentInfoState
{
    DBROOT_EXTENT_PARTIAL_EXTENT  = 1,
    DBROOT_EXTENT_EMPTY_DBROOT    = 2,
    DBROOT_EXTENT_EXTENT_BOUNDARY = 3
};

//------------------------------------------------------------------------------
/** @brief Tracks the current DBRoot/extent we are loading.
 */
//------------------------------------------------------------------------------
struct DBRootExtentInfo
{
    uint32_t              fPartition;
    uint16_t              fDbRoot;
    uint16_t              fSegment;
    BRM::LBID_t           fStartLbid;
    HWM                   fLocalHwm;
    uint64_t              fDBRootTotalBlocks;
    DBRootExtentInfoState fState;

    DBRootExtentInfo() :
        fPartition(0),
        fDbRoot(0),
        fSegment(0),
        fStartLbid(0),
        fLocalHwm(0),
        fDBRootTotalBlocks(0),
        fState(DBROOT_EXTENT_PARTIAL_EXTENT) { }

    DBRootExtentInfo(int colWidth,
        uint16_t    dbRoot,
        uint32_t    partition,
        uint16_t    segment,
        BRM::LBID_t startLbid,
        HWM         localHwm,
        uint64_t    dbrootTotalBlocks);

    bool operator<(const DBRootExtentInfo& entry) const;
};

//------------------------------------------------------------------------------
/** @brief Class to track the order of placement (rotation) of extents as
 *  they are filled in and/or added to the DBRoots for the relevant PM.
 */
//------------------------------------------------------------------------------
class DBRootExtentTracker
{
public:

    /** @brief DBRootExtentTracker constructor
     * @param oid Column OID of interest.
     * @param colWidth Width (in bytes) of the relevant column.
     * @param logger Logger to be used for logging messages.
     * @param vector Collection of DBRoots to be tracked or rotated through.
     */
    EXPORT DBRootExtentTracker ( OID oid,
        int  colWidth,
        Log* logger,
        const BRM::EmDbRootHWMInfo_v& emDbRootHWMInfo );

    /** @brief Select the first DBRoot/segment file to add rows to for this PM.
     * @param dbRootExtent Dbroot/segment file selected for first set of rows.
     * @param bFirstExtentOnThisPM Indicate if this is the 1st extent on this PM
     * @return Returns NO_ERROR if success, else returns error code.
     */
    EXPORT int selectFirstSegFile ( DBRootExtentInfo& dbRootExtent,
                             bool& bFirstExtentOnThisPM,
                             std::string& errMsg );

    /** @brief Set up this Tracker to select the same first DBRoot/segment file
     * as the reference DBRootExtentTracker that is specified.
     *
     * Application code should call selectFirstSegFile for a reference column,
     * and assignFirstSegFile for all other columns in the same table.
     * @param refTracker Tracker object used to assign first DBRoot/segment.
     * @param dbRootExtent Dbroot/segment file selected for first set of rows.
     * @param bFirstExtentOnThisPM Indicate if this is the 1st extent on this PM
     */
    EXPORT void assignFirstSegFile( const DBRootExtentTracker& refTracker,
                             DBRootExtentInfo& dbRootExtent,
                             bool& bFirstExtentOnThisPM );

    /** @brief Iterate/return next DBRoot to be used for the next extent.
     *
     * Case 1)
     * If it is the "very" first extent for the specified DBRoot, then the
     * applicable partition and segment number to be used for the first extent,
     * are also returned.
     *
     * Case 2)
     * If the user moves a DBRoot to a different PM, then the next cpimport.bin
     * job on the recepient PM may encounter 2 partially filled in extents.
     * This differs from the norm, where we only have 1 partially filled extent
     * at any given time, on a PM.  When a DBRoot is moved, we may finish an ex-
     * tent on 1 DBRoot, and instead of advancing to start a new extent, we ro-
     * tate to the recently moved DBRoot, and have to first fill in a partilly
     * filled in extent instead of adding a new extent.  Case 2 is intended to
     * cover this use case.
     * In this case, in the middle of an import, if the next extent to receive
     * rows is a partially filled in extent, then the DBRoot, partition, and 
     * segment number for the partial extent are returned.  In addition, the
     * current HWM and starting LBID for the relevant extent are returned.
     *
     * Case 3)
     * If we are just finishing one extent and adding the next extent, then
     * only the DBRoot argument is relevant, telling us where to create the
     * next extent.
     *
     * @param dbRoot DBRoot for the next extent
     * @param partition If first extent on dbRoot (or partial extent), then
     *        this is the partition #
     * @param segment If first extent on dbRoot (or partial extent), then
     *        this is the segment #
     * @param localHwm If partially full extent, then this is current HWM.
     * @param startLbid If partially full extent, then this is starting LBID of
     *         the current HWM extent.
     * @return Returns true if new extent needs to be allocated, returns false
     *         if extent is partially full, and has room for more rows.
     */
    EXPORT bool nextSegFile( uint16_t&  dbRoot,
                    uint32_t&    partition,
                    uint16_t&    segment,
                    HWM&         localHwm,
                    BRM::LBID_t& startLbid );

    /** @brief get the DBRootExtentInfo list
     */
    const std::vector<DBRootExtentInfo>& getDBRootExtentList();

    /** @brief get the CurrentDBRootIdx
     */
    inline const int getCurrentDBRootIdx()
    {
        boost::mutex::scoped_lock lock(fDBRootExtTrkMutex);
        return fCurrentDBRootIdx;
    }

private:
    // Select First DBRoot/segment file on a PM having no extents for fOID
    int  selectFirstSegFileForEmptyPM ( std::string& errMsg );
    void initEmptyDBRoots();                // init ExtentList for empty DBRoots
    void logFirstDBRootSelection() const;

    OID             fOID;                   // applicable colunn OID
    long long       fBlksPerExtent;         // blocks per extent for fOID
    Log*            fLog;                   // logger
    boost::mutex    fDBRootExtTrkMutex;     // mutex to access fDBRootExtentList
    int             fCurrentDBRootIdx;      // Index into fDBRootExtentList,
                                            //   DBRoot where current extent is
                                            //   being added
    std::vector<DBRootExtentInfo> fDBRootExtentList; // List of current pending
                                            //   DBRoot/extents for each DBRoot
                                            //   assigned to the local PM.
    bool            fStartedWithEmptyPM;    // Did this job start with empty PM
    uint16_t        fEmptyPMFirstDbRoot;    // If Empty PM, this is the first
                                            //   DBRoot for first segment file.
};

} //end of namespace

#undef EXPORT

#endif // WE_DBROOTEXTENTTRACKER_H_
