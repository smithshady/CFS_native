/************************************************************************
 * NASA Docket No. GSC-18,924-1, and identified as “Core Flight
 * System (cFS) Stored Command Application version 3.1.1”
 *
 * Copyright (c) 2021 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

#include "cfe.h"
#include "cfe_tbl_filedef.h"

#include "sc_tbldefs.h"      /* defines SC table headers */
#include "sc_platform_cfg.h" /* defines table buffer size */
#include "sc_msgdefs.h"      /* defines SC command code values */
#include "sc_msgids.h"       /* defines SC packet msg ID's */
#include "sc_msg.h"          /* defines SC message structures */

/* Include the respective message headers */
#include "cfe_evs_msg.h"
#include "cfe_es_msg.h"
#include "cf_msg.h"

/* define messages (#include msgids.h doesn't resolve) */
#define CFE_EVS_CMD_MID  0x1801
#define CFE_ES_CMD_MID   0x1806
#define CF_CMD_MID       0x18B3

/* Checksum not required to be passed in due to a flag in sc_internal_cfg */
#define CKSUM_PLACEHOLDER (0xFF)

/* Custom table structure, modify as needed to add desired commands */
typedef struct
{
    /* Enable debug event type */
    SC_RtsEntryHeader_t                 hdr1;
    CFE_EVS_EnableEventTypeCmd_t        cmd1;
    /* Start perf collection */
    SC_RtsEntryHeader_t                 hdr2;
    CFE_ES_StartPerfDataCmd_t           cmd2;
    /* Get app info */
    SC_RtsEntryHeader_t                 hdr3;
    CFE_ES_QueryAllCmd_t                cmd3;
    /* Downlink app info */
    SC_RtsEntryHeader_t                 hdr4;
    CF_TxFileCmd_t                      cmd4;
    /* Get task info */
    SC_RtsEntryHeader_t                 hdr5;
    CFE_ES_QueryAllTasksCmd_t           cmd5;
    /* Downlink task info*/
    SC_RtsEntryHeader_t                 hdr6;
    CF_TxFileCmd_t                      cmd6;
    /* Write syslog */
    SC_RtsEntryHeader_t                 hdr7;
    CFE_ES_WriteSysLogCmd_t             cmd7;
    /* Downlink syslog */
    SC_RtsEntryHeader_t                 hdr8;
    CF_TxFileCmd_t                      cmd8;
    /* Write erlog */
    SC_RtsEntryHeader_t                 hdr9;
    CFE_ES_WriteERLogCmd_t              cmd9;
    /* Downlink erlog */
    SC_RtsEntryHeader_t                 hdr10;
    CF_TxFileCmd_t                      cmd10;
    /* Downlink functional test log - only exists if make run with unit tests or if the cfe_testcase app was started during runtime */
    SC_RtsEntryHeader_t                 hdr11;
    CF_TxFileCmd_t                      cmd11;
    /* Stop and write perf data */
    SC_RtsEntryHeader_t                 hdr12;
    CFE_ES_StopPerfDataCmd_t            cmd12;
    /* Downlink perf data */
    SC_RtsEntryHeader_t                 hdr13;
    CF_TxFileCmd_t                      cmd13;

} SC_RtsStruct003_t;

/* Define the union to size the table correctly */
typedef union
{
    SC_RtsStruct003_t rts;
    uint16            buf[SC_RTS_BUFF_SIZE];
} SC_RtsTable003_t;

/* Helper macro to get size of structure elements */
#define SC_MEMBER_SIZE(member) (sizeof(((SC_RtsStruct003_t *)0)->member))

/* Used designated initializers to be verbose, modify as needed/desired */
SC_RtsTable003_t SC_Rts003 = {

    /* Enable debug messages */
    .rts.hdr1.TimeTag = 0,
    .rts.cmd1.CommandHeader = CFE_MSG_CMD_HDR_INIT(CFE_EVS_CMD_MID, SC_MEMBER_SIZE(cmd1), CFE_EVS_ENABLE_EVENT_TYPE_CC, CKSUM_PLACEHOLDER),
    .rts.cmd1.Payload.BitMask = (uint8)1,
    .rts.cmd1.Payload.Spare = 0,

    /* Start performance log collection */
    .rts.hdr2.TimeTag = 1,
    .rts.cmd2.CommandHeader = CFE_MSG_CMD_HDR_INIT(CFE_ES_CMD_MID, SC_MEMBER_SIZE(cmd2), CFE_ES_START_PERF_DATA_CC, CKSUM_PLACEHOLDER),
    .rts.cmd2.Payload.TriggerMode = (uint32)1,

    /* Query and write all apps info */
    .rts.hdr3.TimeTag = 2,
    .rts.cmd3.CommandHeader = CFE_MSG_CMD_HDR_INIT(CFE_ES_CMD_MID, SC_MEMBER_SIZE(cmd3), CFE_ES_QUERY_ALL_CC, CKSUM_PLACEHOLDER),
    .rts.cmd3.Payload.FileName = "",

    /* Downlink apps info */
    .rts.hdr4.TimeTag = 4,
    .rts.cmd4.CommandHeader = CFE_MSG_CMD_HDR_INIT(CF_CMD_MID, SC_MEMBER_SIZE(cmd4), CF_TX_FILE_CC, CKSUM_PLACEHOLDER),
    .rts.cmd4.Payload = {(uint8)0, (uint8)1, (uint8)0, (uint8)10, (uint32)2, "/ram/cfe_es_app_info.log", "/cfe_es_app_info.log"},

    /* Query and write all tasks info */
    .rts.hdr5.TimeTag = 5,
    .rts.cmd5.CommandHeader = CFE_MSG_CMD_HDR_INIT(CFE_ES_CMD_MID, SC_MEMBER_SIZE(cmd5), CFE_ES_QUERY_ALL_TASKS_CC, CKSUM_PLACEHOLDER),
    .rts.cmd5.Payload.FileName = "",

    /* Downlink tasks info */
    .rts.hdr6.TimeTag = 9,
    .rts.cmd6.CommandHeader = CFE_MSG_CMD_HDR_INIT(CF_CMD_MID, SC_MEMBER_SIZE(cmd6), CF_TX_FILE_CC, CKSUM_PLACEHOLDER),
    .rts.cmd6.Payload = {(uint8)0, (uint8)1, (uint8)0, (uint8)10, (uint32)2, "/ram/cfe_es_taskinfo.log", "/cfe_es_taskinfo.log"},

    /* Write syslog */
    .rts.hdr7.TimeTag = 10,
    .rts.cmd7.CommandHeader = CFE_MSG_CMD_HDR_INIT(CFE_ES_CMD_MID, SC_MEMBER_SIZE(cmd7), CFE_ES_WRITE_SYS_LOG_CC, CKSUM_PLACEHOLDER),
    .rts.cmd7.Payload.FileName = "",

    /* Downlink syslog */
    .rts.hdr8.TimeTag = 15,
    .rts.cmd8.CommandHeader = CFE_MSG_CMD_HDR_INIT(CF_CMD_MID, SC_MEMBER_SIZE(cmd8), CF_TX_FILE_CC, CKSUM_PLACEHOLDER),
    .rts.cmd8.Payload = {(uint8)0, (uint8)1, (uint8)0, (uint8)10, (uint32)2, "/ram/cfe_es_syslog.log", "/cfe_es_syslog.log"},

    /* Write ER log */
    .rts.hdr9.TimeTag = 16,
    .rts.cmd9.CommandHeader = CFE_MSG_CMD_HDR_INIT(CFE_ES_CMD_MID, SC_MEMBER_SIZE(cmd9), CFE_ES_WRITE_ER_LOG_CC, CKSUM_PLACEHOLDER),
    .rts.cmd9.Payload.FileName = "",

    /* Downlink erlog */
    .rts.hdr10.TimeTag = 20,
    .rts.cmd10.CommandHeader = CFE_MSG_CMD_HDR_INIT(CF_CMD_MID, SC_MEMBER_SIZE(cmd10), CF_TX_FILE_CC, CKSUM_PLACEHOLDER),
    .rts.cmd10.Payload = {(uint8)0, (uint8)1, (uint8)0, (uint8)10, (uint32)2, "/ram/cfe_erlog.log", "/cfe_erlog.log"},

    /* Downlink functional test log */
    .rts.hdr11.TimeTag = 25,
    .rts.cmd11.CommandHeader = CFE_MSG_CMD_HDR_INIT(CF_CMD_MID, SC_MEMBER_SIZE(cmd11), CF_TX_FILE_CC, CKSUM_PLACEHOLDER),
    .rts.cmd11.Payload = {(uint8)0, (uint8)1, (uint8)0, (uint8)10, (uint32)2, "/cf/cfe_test.log", "/cfe_test.log"},

    /* Stop performance log collection */
    .rts.hdr12.TimeTag = 35,
    .rts.cmd12.CommandHeader = CFE_MSG_CMD_HDR_INIT(CFE_ES_CMD_MID, SC_MEMBER_SIZE(cmd12), CFE_ES_STOP_PERF_DATA_CC, CKSUM_PLACEHOLDER),
    .rts.cmd12.Payload.DataFileName = "",

    /* Downlink performance test log */
    .rts.hdr13.TimeTag = 42,
    .rts.cmd13.CommandHeader = CFE_MSG_CMD_HDR_INIT(CF_CMD_MID, SC_MEMBER_SIZE(cmd13), CF_TX_FILE_CC, CKSUM_PLACEHOLDER),
    .rts.cmd13.Payload = {(uint8)0, (uint8)1, (uint8)0, (uint8)10, (uint32)2, "/ram/cfe_es_perf.dat", "/cfe_es_perf.dat"}

};

/* Macro for table structure */
CFE_TBL_FILEDEF(SC_Rts003, SC.RTS_TBL003, SC Example RTS_TBL003, sc_rts003.tbl)
