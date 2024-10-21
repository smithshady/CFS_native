/**
 * @file atsam_test_cmds.c
 * @author Vincent Marias <vincent.marias@lunaroutpost.com>
 * @desc Implementations for command handler functions.
 */

/* private headers */
#include "atsam_test_cmds.h"
#include "atsam_test_app.h"

/* public headers */
#include "atsam_test_eventids.h"
#include "atsam_test_fcncodes.h"

/* config headers */
#include "atsam_test_tbldefs.h"

CFE_Status_t ATSAM_TEST_ProcessTlmReqCmd(const struct ATSAM_TEST_HdrOnlyPkt*
        const msg) {
    CFE_Status_t status = CFE_SUCCESS;  /* exit status */
    CFE_StatusString_t status_str;      /* status string buffer */

    /* get values from table(s) */
    struct ATSAM_TEST_Table1_t* tbl1;
    status = CFE_TBL_GetAddress((void**)&tbl1, ATSAM_TEST_Data.Tbl1);

    if (status >= CFE_SUCCESS) {
        /* save values */
        ATSAM_TEST_Data.TblVal1 = tbl1->Val1;
        ATSAM_TEST_Data.TblVal2 = tbl1->Val2;

        /* release table pointer */
        CFE_TBL_ReleaseAddress(ATSAM_TEST_Data.Tbl1);

        /* CFE_EVS_SendEvent(ATSAM_TEST_INIT_INF_EID,
                CFE_EVS_EventType_INFORMATION, "Retrieved values from Table "
                "1."); */
    } else {
        CFE_ES_StatusToString(status, &status_str);
        CFE_EVS_SendEvent(ATSAM_TEST_TBL_ERR_EID, CFE_EVS_EventType_CRITICAL,
                "Error loading values from Table 1, RC = %s", status_str);

        return status;
    }

    /* gather/packetize app telemetry */
    struct ATSAM_TEST_HkTlmPkt pkt;
    CFE_MSG_Init(CFE_MSG_PTR(pkt.TlmHdr),
            CFE_SB_ValueToMsgId(ATSAM_TEST_HK_TLM_MID), sizeof pkt);

    pkt.Payload.CmdCnt  = ATSAM_TEST_Data.CmdCnt;
    pkt.Payload.ErrCnt  = ATSAM_TEST_Data.ErrCnt;
    pkt.Payload.TblVal1 = ATSAM_TEST_Data.TblVal1;
    pkt.Payload.TblVal2 = ATSAM_TEST_Data.TblVal2;

    /* send telemetry packet */
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(pkt.TlmHdr));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(pkt.TlmHdr), true);

    return CFE_SUCCESS;
}

CFE_Status_t ATSAM_TEST_ProcessNoopCmd(const struct ATSAM_TEST_HdrOnlyPkt*
        const msg) {

    /* send network packet */
    struct ATSAM_TEST_HdrOnlyPkt pkt;
    CFE_MSG_Init(CFE_MSG_PTR(pkt.CmdHdr),
            CFE_SB_ValueToMsgId(ATSAM_TEST_SBN_TEST), sizeof pkt);
    CFE_MSG_SetFcnCode(CFE_MSG_PTR(pkt.CmdHdr), ATSAM_TEST_SEND_NETWORK);
    CFE_SB_TransmitMsg(CFE_MSG_PTR(pkt.CmdHdr), true);

    CFE_EVS_SendEvent(ATSAM_TEST_NOOP_EID, CFE_EVS_EventType_INFORMATION,
            "Received NOOP, sent network packet");

    return CFE_SUCCESS;
}

CFE_Status_t ATSAM_TEST_ProcessResetCountersCmd(const struct
        ATSAM_TEST_HdrOnlyPkt* const msg) {
    CFE_EVS_SendEvent(ATSAM_TEST_RESET_COUNTERS_EID,
            CFE_EVS_EventType_INFORMATION, "Received \"Reset Counters\" "
            "command");

    ATSAM_TEST_Data.CmdCnt = 0;
    ATSAM_TEST_Data.ErrCnt = 0;

    return CFE_SUCCESS;
}

CFE_Status_t ATSAM_TEST_ProcessDumpPayloadCmd(const struct
        ATSAM_TEST_DumpPayloadPkt* const msg) {
    /* get the string out of the message (might not be null-terminated, use
     * CFE API */
    char payload_str[ATSAM_TEST_STR_LEN];
    CFE_SB_MessageStringGet(payload_str, msg->Payload.Str, "",
            ATSAM_TEST_STR_LEN, ATSAM_TEST_STR_LEN);

    CFE_EVS_SendEvent(ATSAM_TEST_DUMP_PAYLOAD_EID,
            CFE_EVS_EventType_INFORMATION, "Received payload: \"%s\"",
            payload_str);

    return CFE_SUCCESS;
}
