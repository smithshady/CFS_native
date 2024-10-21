/**
 * @file atsam_test_dispatch.c
 * @author Vincent Marias <vincent.marias@lunaroutpost.com>
 * @desc High-level command dispatch functions. These call into the more
 * specific handlers in atsam_test_cmds.(h|c).
 */

/* private headers */
#include "atsam_test_dispatch.h"
#include "atsam_test_cmds.h"
#include "atsam_test_app.h"

/* public headers */
#include "atsam_test_eventids.h"

/* config headers */
#include "atsam_test_fcncodes.h"

void ATSAM_TEST_ProcessCmdPipe(const CFE_SB_Buffer_t* const SBBufPtr) {
    ++ATSAM_TEST_Data.CmdCnt;

    /* get the message ID */
    CFE_SB_MsgId_t mid = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &mid);

    /* figure out what to do */
    switch (CFE_SB_MsgIdToValue(mid)) {
        case ATSAM_TEST_SBN_TEST:
            ATSAM_TEST_ProcessSBNCmd(SBBufPtr);
            break;
        case ATSAM_TEST_CMD_MID:        /* received ground command */
            ATSAM_TEST_ProcessGndCmd(SBBufPtr);
            break;
        case ATSAM_TEST_SEND_HK_MID:    /* received request for telemetry */
            ATSAM_TEST_ProcessTlmReqCmd((const struct ATSAM_TEST_HdrOnlyPkt* const) SBBufPtr);
            break;
        default:                        /* unknown message ID */
            ++ATSAM_TEST_Data.ErrCnt;

            CFE_EVS_SendEvent(ATSAM_TEST_MID_ERR_EID, CFE_EVS_EventType_ERROR,
                    "Invalid command packet, MID = 0x%x", (unsigned)
                    CFE_SB_MsgIdToValue(mid));
            break;
    }
}

void ATSAM_TEST_ProcessSBNCmd(const CFE_SB_Buffer_t* const SBBufPtr) {
    CFE_EVS_SendEvent(ATSAM_TEST_NOOP_EID, CFE_EVS_EventType_INFORMATION,
            "Received message from across the network!");
}

void ATSAM_TEST_ProcessGndCmd(const CFE_SB_Buffer_t* const SBBufPtr) {
    /* get the command code */
    CFE_MSG_FcnCode_t cc = 0;
    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &cc);

    /* figure out which commmand to process */
    switch (cc) {
        case ATSAM_TEST_NOOP_CC:
            ATSAM_TEST_ProcessNoopCmd((const struct ATSAM_TEST_HdrOnlyPkt* const) SBBufPtr);
            break;
        case ATSAM_TEST_RESET_COUNTERS_CC:
            ATSAM_TEST_ProcessResetCountersCmd((const struct ATSAM_TEST_HdrOnlyPkt* const) SBBufPtr);
            break;
        case ATSAM_TEST_DUMP_PAYLOAD_CC:
            ATSAM_TEST_ProcessDumpPayloadCmd((const struct ATSAM_TEST_DumpPayloadPkt* const) SBBufPtr);
            break;
        case ATSAM_TEST_SEND_NETWORK:
            printf("Got send network command");
            break;
        default:
            ++ATSAM_TEST_Data.ErrCnt;
            CFE_EVS_SendEvent(ATSAM_TEST_CMD_ERR_EID,
                    CFE_EVS_EventType_ERROR, "Received invalid ground "
                    "command, CC = %d", cc);
    }
}
