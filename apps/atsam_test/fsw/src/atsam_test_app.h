/**
 * @file atsam_test_app.h
 * @author Vincent Marias <vincent.marias@lunaroutpost.com>
 * @desc Private header that defines data structures and functions for app
 * startup. Includes the main entry point for the app.
 */

#ifndef ATSAM_TEST_APP_H
#define ATSAM_TEST_APP_H

#include "cfe.h"
#include "cfe_config.h"
#include "atsam_test_msgstruct.h"

/* Global data -------------------------------------------------------------- */

struct ATSAM_TEST_Data_t {
    uint32 RunStatus;

    CFE_SB_PipeId_t CmdPipe;    /* handle to command pipe */
    CFE_TBL_Handle_t Tbl1;      /* handle to table1 */

    /* housekeeping telemetry */
    uint8 CmdCnt, ErrCnt;
    uint16 TblVal1, TblVal2;
};

extern struct ATSAM_TEST_Data_t ATSAM_TEST_Data;

/* Local functions ---------------------------------------------------------- */

/**
 * @brief Application entry point; main process loop.
 */
void ATSAM_TEST_AppMain(void);

/**
 * @brief One-time app initialization.
 */
CFE_Status_t ATSAM_TEST_Init(void);

#endif /* ATSAM_TEST_APP_H */
