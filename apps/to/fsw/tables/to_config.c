/*==============================================================================
** File Name: to_config.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** Title:     TO table definition
**
** $Author: $
** $Revision: $
** $Date:  $
**
** Purpose:   To provide the table for default data config.
**
** Functions Contained:
**    None
**
**
** Limitations, Assumptions, External Events, and Notes:
**  1.   None
**
**
**==============================================================================
*/

/*
#ifndef _TO_CONFIG_
#define _TO_CONFIG_


#ifdef   __cplusplus
extern "C" {
#endif
*/

/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "to_platform_cfg.h"
#include "to_mission_cfg.h"
#include "to_app.h"
#include "to_tbldefs.h"
#include "to_grpids.h"

#include "cfe_msgids.h"

#include "to_msgids.h"
#include "ci_msgids.h"
#include "cf_msgids.h"
#include "sch_msgids.h"
#include "hs_msgids.h"
#include "hk_msgids.h"
#include "sch_msgids.h"


/*
** Local Defines
*/


/*
** Local Structure Declarations
*/

static CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    "to_ConfigTable", "TO.to_config", "TO config table",
    "to_config.tbl", sizeof(TO_ConfigTable_t)
};

/*
** Default TO iLoad table data
*/

// TODO update MIDs as needed
TO_ConfigTable_t to_ConfigTable =
{
   {
       /* 0 - 9 */
       //{CF_CONFIG_TLM_MID,            {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{CF_HK_TLM_MID},                {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       //{CF_SPACE_TO_GND_PDU_MID,      {0,0},  32,  0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       //{CF_TRANS_TLM_MID,             {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{CFE_ES_APP_TLM_MID},           {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{CFE_ES_HK_TLM_MID},            {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{CFE_ES_MEMSTATS_TLM_MID},      {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       //{CFE_ES_SHELL_TLM_MID,         {0,0},  32,  0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       //{CFE_EVS_EVENT_MSG_MID,        {0,0},  32,  0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{CFE_EVS_HK_TLM_MID},           {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
                                                         
       /* 10 - 19 */                                     
       {{CFE_SB_ALLSUBS_TLM_MID},       {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{CFE_SB_HK_TLM_MID},            {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{CFE_SB_ONESUB_TLM_MID},        {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{CFE_SB_STATS_TLM_MID},         {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{CFE_TBL_HK_TLM_MID},           {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{CFE_TBL_REG_TLM_MID},          {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{CFE_TIME_DIAG_TLM_MID},        {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{CFE_TIME_HK_TLM_MID},          {0,0},  1,   0xffff,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {{TO_HK_TLM_MID},                {0,0},  1,   0xffff,     TO_GROUP_APP | TO_MGROUP_ONE, 0,1},
       {{HS_HK_TLM_MID},                {0,0},  1,   0xffff,     TO_GROUP_APP | TO_MGROUP_ONE, 0,1},
                                                         
       /* 20 - 29 */                                     
       {{SCH_DIAG_TLM_MID},             {0,0},  1,   0xffff,     TO_GROUP_APP | TO_MGROUP_ONE, 0,1},
       {{SCH_HK_TLM_MID},               {0,0},  1,   0xffff,     TO_GROUP_APP | TO_MGROUP_ONE, 0,1},
       {{HK_HK_TLM_MID},                {0,0},  1,   0xffff,     TO_GROUP_APP | TO_MGROUP_ONE, 0,1},
       {{CI_HK_TLM_MID},                {0,0},  1,   0xffff,     TO_GROUP_APP | TO_MGROUP_ONE, 0,1},
       {{TO_DATA_TYPE_MID},             {0,0},  1,   0xffff,     TO_GROUP_APP | TO_MGROUP_ONE, 0,1},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       
       
       /* 30 - 39 */
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 40 - 49 */
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 50 - 59 */
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 60 - 69 */
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 70 - 79 */
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 80 - 89 */
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 90 - 99 */
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {{TO_UNUSED_ENTRY},              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0}
    }
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/

/*
#ifdef   __cplusplus
}
#endif

#endif
*/

/* _TO_CONFIG_ */
