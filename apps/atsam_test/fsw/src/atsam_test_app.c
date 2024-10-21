/**
 * @file atsam_test_app.c
 * @author Vincent Marias <vincent.marias@lunaroutpost.com>
 * @desc Implementation of data structures and functions for app startup.
 * Includes the main entry point for the app.
 */

#include <strings.h> /* bzero */

/* private headers */
#include "atsam_test_app.h"
#include "atsam_test_dispatch.h"

/* public headers */
#include "atsam_test_eventids.h"

/* config headers */
#include "atsam_test_internal_cfg.h"
#include "atsam_test_msgids.h"
#include "atsam_test_tbldefs.h"

/* Global data -------------------------------------------------------------- */

struct ATSAM_TEST_Data_t ATSAM_TEST_Data;

/* Local functions ---------------------------------------------------------- */

void ATSAM_TEST_AppMain(void) {
    CFE_Status_t status = CFE_SUCCESS;  /* exit status */
    CFE_StatusString_t status_str;      /* status string buffer */
    CFE_SB_Buffer_t* SBBufPtr;          /* SB message buffer */

    /* app-specific inititialization */
    status = ATSAM_TEST_Init();

    if (status != CFE_SUCCESS) {
        /* app init failed, run loop should not start */
        ATSAM_TEST_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /* process run loop */
    while (CFE_ES_RunLoop(&ATSAM_TEST_Data.RunStatus) == true) {
        //printf("%s: ATSAM_TEST app running :3\n", __func__);
        
        /* pend on the command pipe until something comes in */
        status = CFE_SB_ReceiveBuffer(&SBBufPtr, ATSAM_TEST_Data.CmdPipe, CFE_SB_PEND_FOREVER);

        /* we received a message ! */
        if (status == CFE_SUCCESS) {
            //printf("%s: Message received!\n", __func__);
            ATSAM_TEST_ProcessCmdPipe(SBBufPtr);
        } else {
            CFE_ES_StatusToString(status, &status_str);
            CFE_EVS_SendEvent(ATSAM_TEST_PIPE_ERR_EID,
                    CFE_EVS_EventType_CRITICAL, "Error reading from message "
                    "pipe, RC = %s, exiting...", status_str);

            /* exit the app now */
            ATSAM_TEST_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    /* kill the app */
    CFE_ES_ExitApp(ATSAM_TEST_Data.RunStatus);
}

CFE_Status_t ATSAM_TEST_Init(void) {
    CFE_Status_t status = CFE_SUCCESS;  /* exit status */
    CFE_StatusString_t status_str;      /* status string buffer */

    /* zero global data */
    bzero(&ATSAM_TEST_Data, sizeof ATSAM_TEST_Data);

    /* initial run status is good */
    ATSAM_TEST_Data.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /* register events */
    status = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);

    if (status == CFE_SUCCESS) {
        CFE_EVS_SendEvent(ATSAM_TEST_INIT_INF_EID,
                CFE_EVS_EventType_INFORMATION, "Events registered.");
    } else {
        CFE_ES_StatusToString(status, &status_str);
        CFE_ES_WriteToSysLog("Error registering events, RC = %s\n",
                status_str);

        return status;
    }

    /* create command pipe */
    status = CFE_SB_CreatePipe(&ATSAM_TEST_Data.CmdPipe, ATSAM_TEST_PIPE_DEPTH,
            ATSAM_TEST_PIPE_NAME);

    if (status == CFE_SUCCESS) {
        CFE_EVS_SendEvent(ATSAM_TEST_INIT_INF_EID,
                CFE_EVS_EventType_INFORMATION, "Command pipe created.");
    } else {
        CFE_ES_StatusToString(status, &status_str);
        CFE_EVS_SendEvent(ATSAM_TEST_PIPE_ERR_EID,
                CFE_EVS_EventType_CRITICAL, "Error creating command pipe, RC "
                "= %s", status_str);

        return status;
    }

    /* subscribe to housekeeping request packets */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(ATSAM_TEST_SEND_HK_MID),
            ATSAM_TEST_Data.CmdPipe);

    if (status == CFE_SUCCESS) {
        CFE_EVS_SendEvent(ATSAM_TEST_INIT_INF_EID,
                CFE_EVS_EventType_INFORMATION, "Subscribed to housekeeping "
                "requests.");
    } else {
        CFE_ES_StatusToString(status, &status_str);
        CFE_EVS_SendEvent(ATSAM_TEST_PIPE_ERR_EID,
                CFE_EVS_EventType_CRITICAL, "Error subscribing to "
                "housekeeping requests, RC = %s", status_str);

        return status;
    }

    /* subscribe to ground command packets */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(ATSAM_TEST_CMD_MID),
            ATSAM_TEST_Data.CmdPipe);

    if (status == CFE_SUCCESS) {
        CFE_EVS_SendEvent(ATSAM_TEST_INIT_INF_EID,
                CFE_EVS_EventType_INFORMATION, "Subscribed to ground command "
                "packets.");
    } else {
        CFE_ES_StatusToString(status, &status_str);
        CFE_EVS_SendEvent(ATSAM_TEST_PIPE_ERR_EID,
                CFE_EVS_EventType_CRITICAL, "Error subscribing to ground "
                "command packets, RC = %s",  status_str);

        return status;
    }

    /* subscribe to network packets */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(ATSAM_TEST_SBN_TEST),
            ATSAM_TEST_Data.CmdPipe);

    if (status == CFE_SUCCESS) {
        CFE_EVS_SendEvent(ATSAM_TEST_INIT_INF_EID,
                CFE_EVS_EventType_INFORMATION, "Subscribed to network packets");
    } else {
        CFE_ES_StatusToString(status, &status_str);
        CFE_EVS_SendEvent(ATSAM_TEST_PIPE_ERR_EID,
                CFE_EVS_EventType_CRITICAL, "Error subscribing to network packets, RC = %s",  status_str);
        return status;
    }

    /* register table(s) */
    status = CFE_TBL_Register(&ATSAM_TEST_Data.Tbl1, "Table1", sizeof(struct
                ATSAM_TEST_Table1_t), CFE_TBL_OPT_DEFAULT, NULL);

    if (status == CFE_SUCCESS) {
        CFE_EVS_SendEvent(ATSAM_TEST_INIT_INF_EID,
                CFE_EVS_EventType_INFORMATION, "Registered Table 1.");
    } else {
        CFE_ES_StatusToString(status, &status_str);
        CFE_EVS_SendEvent(ATSAM_TEST_TBL_ERR_EID, CFE_EVS_EventType_CRITICAL,
                "Error registering Table 1, RC = %s", status_str);

        return status;
    }

    /* load table image */
    status = CFE_TBL_Load(ATSAM_TEST_Data.Tbl1, CFE_TBL_SRC_FILE,
            ATSAM_TEST_TABLE1_FILE);
    
    if (status == CFE_SUCCESS) {
        CFE_EVS_SendEvent(ATSAM_TEST_INIT_INF_EID,
                CFE_EVS_EventType_INFORMATION, "Loaded default image for "
                "Table 1 from file.");
    } else {
        CFE_ES_StatusToString(status, &status_str);
        CFE_EVS_SendEvent(ATSAM_TEST_TBL_ERR_EID, CFE_EVS_EventType_CRITICAL,
                "Error loading default image for Table 1, RC = %s",
                status_str);

        return status;
    }


    /* all is good! (probably) */
    CFE_EVS_SendEvent(ATSAM_TEST_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,
            "Application initialized.");

    return status;
}
