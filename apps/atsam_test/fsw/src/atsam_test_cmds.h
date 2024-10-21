/**
 * @file atsam_test_cmds.h
 * @author Vincent Marias <vincent.marias@lunaroutpost.com>
 * @desc Private header defining command handler functions.
 */

#ifndef ATSAM_TEST_CMDS_H
#define ATSAM_TEST_CMDS_H

#include "cfe.h"
#include "atsam_test_msgids.h"
#include "atsam_test_msgstruct.h"

/* Telemetry Commands ------------------------------------------------------- */

/**
 * @brief Handle a request for housekeeping telemetry by sending the
 * telemetry packet.
 */
CFE_Status_t ATSAM_TEST_ProcessTlmReqCmd(const struct ATSAM_TEST_HdrOnlyPkt*
        const msg);

/* Ground Commands ---------------------------------------------------------- */

/**
 * @brief Handle a NOOP command (does nothing but send an event).
 */
CFE_Status_t ATSAM_TEST_ProcessNoopCmd(const struct ATSAM_TEST_HdrOnlyPkt*
        const msg);

/**
 * @brief "Handle Reset" Counters command (sets command and error counters to
 * 0).
 */
CFE_Status_t ATSAM_TEST_ProcessResetCountersCmd(const struct
        ATSAM_TEST_HdrOnlyPkt* const msg);

/**
 * @brief Handle "Dump Payload" command (prints the provided string to the
 * screen).
 */
CFE_Status_t ATSAM_TEST_ProcessDumpPayloadCmd(const struct
        ATSAM_TEST_DumpPayloadPkt* const msg);

#endif /* ATSAM_TEST_CMDS_H */
