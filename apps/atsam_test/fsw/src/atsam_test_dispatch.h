/**
 * @file atsam_test_dispatch.h
 * @author Vincent Marias <vincent.marias@lunaroutpost.com>
 * @desc Private header that describes command dispatch functions.
 */

#ifndef ATSAM_TEST_DISPATCH_H
#define ATSAM_TEST_DISPATCH_H

#include "cfe.h"
#include "atsam_test_msgids.h"

/**
 * @brief Processes all incoming packets on the CmdPipe message pipe. Calls
 * functions to either process a ground command or a housekeeping request, or
 * prints and error message.
 */
void ATSAM_TEST_ProcessCmdPipe(const CFE_SB_Buffer_t* const SBBufPtr);

/**
 * @brief Processes ground commands by calling the correct handler function
 * depending on the command type.
 */
void ATSAM_TEST_ProcessGndCmd(const CFE_SB_Buffer_t* const SBBufPtr);

void ATSAM_TEST_ProcessSBNCmd(const CFE_SB_Buffer_t* const SBBufPtr);

#endif /* ATSAM_TEST_DISPATCH_H */
