/**
 * @file default_atsam_test_mdgdefs.h
 * @author Vincent Marias <vincent.marias@lunaroupost.com>
 * @desc Definitions for the CMD/TLM message payload(s) of the app.
 */

#ifndef ATSAM_TEST_DEFAULT_ATSAM_TEST_MSGDEFS_H
#define ATSAM_TEST_DEFAULT_ATSAM_TEST_MSGDEFS_H

#include "common_types.h"
#include "atsam_test_internal_cfg.h"

/**
 * @brief HouseKeeping Telemetry Packet Payload
 */
struct ATSAM_TEST_HkTlm_Payload {
    uint8 CmdCnt, ErrCnt;
    uint16 TblVal1, TblVal2;
};

/**
 * @brief Payload to test sending a command with a payload
 */
struct ATSAM_TEST_Dump_Payload {
    char Str[ATSAM_TEST_STR_LEN];
};

#endif /* ATSAM_TEST_DEFAULT_ATSAM_TEST_MSGDEFS_H */
