/**
 * @file default_atsam_test_msgids.h
 * @author Vincent Marias <vincent.marias@lunaroutpost.com>
 * @desc CFE Software Bus Message ID definitions for CMD/TLM interface(s) of
 * the ATSAM Test app.
 */

#ifndef ATSAM_TEST_DEFAULT_ATSAM_TEST_MSGIDS_H
#define ATSAM_TEST_DEFAULT_ATSAM_TEST_MSGIDS_H

#include "cfe_core_api_base_msgids.h" /* CFE_PLATOFRM_(CMD|TLM)_TOPICID_TO_MIDV */

#include "atsam_test_topicids.h"

/* Command MIDs */
#define ATSAM_TEST_CMD_MID      CFE_PLATFORM_CMD_TOPICID_TO_MIDV(CFE_MISSION_ATSAM_TEST_CMD_TOPICID)        /* 0x1887 */
#define ATSAM_TEST_SEND_HK_MID  CFE_PLATFORM_CMD_TOPICID_TO_MIDV(CFE_MISSION_ATSAM_TEST_SEND_HK_TOPICID)    /* 0x1894 */

/* Telemetry MIDs */
#define ATSAM_TEST_HK_TLM_MID  CFE_PLATFORM_TLM_TOPICID_TO_MIDV(CFE_MISSION_ATSAM_TEST_HK_TLM_TOPICID)      /* 0x0894 */

/* Network MIDs */
#define ATSAM_TEST_SBN_TEST    CFE_PLATFORM_CMD_TOPICID_TO_MIDV(CFE_MISSION_ATSAM_TEST_SBN_TEST)            /* 0x1893 */

#endif /* ATSAM_TEST_DEFAULT_ATSAM_TEST_MSGIDS_H */
