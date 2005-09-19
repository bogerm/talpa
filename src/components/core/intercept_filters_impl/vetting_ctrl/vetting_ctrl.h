/*
 * vetting_ctrl.h
 *
 * TALPA Filesystem Interceptor
 *
 * Copyright (C) 2004 Sophos Plc, Oxford, England.
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License Version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not,
 * write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */
#ifndef H_VETTINGCTRL
#define H_VETTINGCTRL


#include <asm/atomic.h>
#include <linux/wait.h>

#include "common/locking.h"
#include "common/list.h"
#include "intercept_filters/iintercept_filter.h"
#include "vetting_server/ivetting_server.h"
#include "configurator/iconfigurable.h"
#include "filesystem/ifilesystem_factory.h"
#include "process_and_thread/ithreadandprocess_factory.h"

/*
 * Configuration structures
 */


#define MAX_STREAM_PACKET_SIZE (32*1024)
#define MIN_STREAM_PACKET_SIZE (1*1024)


#define VETCTRL_CFGDATASIZE      (16)

typedef struct {
    char    name[VETCTRL_CFGDATASIZE];
    char    value[VETCTRL_CFGDATASIZE];
} VetCtrlConfigData;

typedef struct {
    char    name[VETCTRL_CFGDATASIZE];
    char    value[PATH_MAX];
} VetCtrlRoutingConfigData;

typedef enum {
    FILESYSTEM = 1, /* Don't change this to zero! */
    PATH
} EVetCtrlRoutingType;

typedef struct
{
    talpa_list_head     head;
    EVetCtrlRoutingType type;
    char*               string;
    unsigned int        len;
    unsigned int        group;
} VetCtrlConfigObject;



typedef struct tag_VettingController
{
    IInterceptFilter          i_IInterceptFilter;
    IVettingServer            i_IVettingServer;
    IConfigurable             i_IConfigurable;
    void                      (*delete)(struct tag_VettingController* object);
    bool                      mEnabled;
    talpa_simple_lock_t       mVettingIDLock;
    uint32_t                  mNextVettingID;
    talpa_rcu_lock_t          mClientsLock;
    talpa_list_head           mClients;
    VettingClientID           mNextClientID;
    VettingGroup              mGroups[8];
    unsigned int              mFOPLookup[6];
    bool                      mXHack;

    talpa_rcu_lock_t          mConfigLock;
    talpa_mutex_t             mConfigSerialize;
    talpa_list_head           mRoutings;
    atomic_t                  mTimeout;
    atomic_t                  mFSTimeout;
    char*                     mRoutingsSet;

    PODConfigurationElement   mConfig[6];
    VetCtrlConfigData         mStateConfigData[1];
    VetCtrlConfigData         mTimeoutConfigData[1];
    VetCtrlConfigData         mFSTimeoutConfigData[1];
    VetCtrlRoutingConfigData  mRoutingConfigData[1];
    VetCtrlConfigData         mXHackConfigData[1];

    IFilesystemFactory*       mFilesystemFactory;
    IThreadAndProcessFactory* mThreadFactory;
} VettingController;

/*
 * Object Creators.
 */
VettingController* newVettingController(void);





#endif

/*
 * End of vetting_ctrl.h
 */

