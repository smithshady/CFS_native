/**
 * @file default_atsam_test_msgstruct.h
 * @author Vincent Marias <vincent.marias@lunaroutpost.com>
 * @desc Structures that define the CMD/TLM message interface(s) of the app.
 */

#ifndef ATSAM_TEST_DEFAULT_ATSAM_TEST_MSGSTRUCT_H
#define ATSAM_TEST_DEFAULT_ATSAM_TEST_MSGSTRUCT_H

#include "atsam_test_msgdefs.h"
#include "cfe_msg_hdr.h"

/**
 * @brief Header-Only Packet (no payload)
 */
struct ATSAM_TEST_HdrOnlyPkt {
    CFE_MSG_CommandHeader_t CmdHdr;
};

/**
 * @brief Packet w/ command header and simple payload
 */
struct ATSAM_TEST_DumpPayloadPkt {
    CFE_MSG_CommandHeader_t CmdHdr;
    struct ATSAM_TEST_Dump_Payload Payload;
};

/**
 * @brief HouseKeeping Telemetry Packet
 */
struct ATSAM_TEST_HkTlmPkt {
    CFE_MSG_TelemetryHeader_t TlmHdr;
    struct ATSAM_TEST_HkTlm_Payload Payload;
};

#endif /* ATSAM_TEST_DEFAULT_ATSAM_TEST_MSGSTRUCT_H */
