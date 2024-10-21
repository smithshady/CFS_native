/************************************************************************
 * NASA Docket No. GSC-18,922-1, and identified as “Core Flight
 * System (cFS) Memory Dwell Application Version 2.4.1”
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

/**
 * @file
 *   Specification for the CFS Memory Dwell command and telemetry
 *   message constant definitions.
 *
 * @note
 *   DO NOT PUT ANY TYPEDEFS OR STRUCTURE DEFINITIONS IN THIS FILE!
 *   ADD THEM TO md_msg.h IF NEEDED! */
#ifndef MD_MSGDEFS_H
#define MD_MSGDEFS_H

#include <md_platform_cfg.h>

/**
 * \defgroup cfsmdcmdcodes CFS Memory Dwell Command Codes
 * \{
 */

/**
 * \brief Memory Dwell No-Op Command
 *
 *  \par Description
 *       This command increments the MD application's
 *       valid command execution counter.
 *
 *  \par Command Structure
 *       #MD_NoArgsCmd_t
 *
 *  \par Command Verification
 *       Successful execution of this command may be verified with the
 *       following telemetry:
 *       - #MD_HkTlm_t.ValidCmdCntr will increment
 *       - The #MD_NOOP_INF_EID informational event message will
 *         be generated
 *
 *  \par Error Conditions
 *       This command may fail for the following reason(s):
 *       - Unexpected command length.
 *
 *       Evidence of an unexpected command length error may be found
 *       in the following telemetry:
 *       - #MD_HkTlm_t.InvalidCmdCntr will increment.
 *       - The #MD_CMD_LEN_ERR_EID error event message will be issued.
 *
 *  \par Criticality
 *       None
 */
#define MD_NOOP_CC 0

/**
 * \brief Memory Dwell Reset Counters Command
 *
 *  \par Description
 *       This command resets the following counters within the
 *       Memory Dwell housekeeping telemetry:
 *       - Command Execution Counter
 *       - Command Error Counter
 *
 *  \par Command Structure
 *       #MD_NoArgsCmd_t
 *
 *  \par Command Verification
 *       Successful execution of this command may be verified with
 *       the following telemetry:
 *       - #MD_HkTlm_t.ValidCmdCntr will be set to zero.
 *       - #MD_HkTlm_t.InvalidCmdCntr will be set to zero.
 *       - The #MD_RESET_CNTRS_DBG_EID debug event message will be generated.
 *
 *  \par Error Conditions
 *       This command may fail for the following reason(s):
 *       - Unexpected command length.
 *
 *       Evidence of an unexpected command length error may be found
 *       in the following telemetry:
 *       - #MD_HkTlm_t.InvalidCmdCntr will increment.
 *       - The #MD_CMD_LEN_ERR_EID error event message will be issued.
 *
 *  \par Criticality
 *       This command is not inherently dangerous.  However, it is
 *       possible for ground systems and on-board safing procedures
 *       to be designed such that they react to changes in the counter
 *       values that are reset by this command.
 */
#define MD_RESET_CNTRS_CC 1

/**
 * \brief Memory Dwell Start Dwell Command
 *
 *  \par Description
 *      This command sets the Enabled flag(s) associated with the Dwell Table(s)
 *      that have been designated by the command's TableMask argument.
 *
 *      When this Enabled flag is set, and the associated Dwell Table has
 *      one or more dwell specifications defined beginning with the Table's
 *      first entry, and the Table has a non-zero value for total delays
 *      (aka as the Rate), dwell processing will occur.
 *
 *      The first dwell occurs on receipt of the first Wakeup Message from
 *      the Scheduler following the Start Dwell Command.  That dwell means
 *      that a value from memory is read, and inserted into the Dwell Packet.
 *
 *      Dwell Packets are issued at a rate specified by the Rate value
 *      associated with the Dwell Table.   The Rate is a multiple of
 *      Wakeup Messages issued from the Scheduler.  The Rate value is calculated
 *      as the sum of all the individual delays specified by individual dwell
 *      entries in a Dwell Table.
 *
 *      Note that the dwell state will not be affected for the Dwell Tables
 *      _not_ designated by the TableMask argument.  Thus, for example, if
 *      Dwell Table #1 has already been started, and a Start Dwell Command is
 *      issued to start Dwell Tables #2 and #3, Dwell Table #1 will still be
 *      in started state following the command.
 *
 *      Note that if this command is issued when the Dwell Table has already
 *      been started, the effect will be to restart the table.  The current
 *      entry will be set to the first entry and any data previously collected
 *      will be lost.
 *
 *      Note that the value of the Enabled flag is also updated when a
 *      Dwell Table is loaded.
 *
 *  \par Command Structure
 *       #MD_CmdStartStop_t
 *
 *  \par Command Verification
 *       Successful execution of this command may be verified with
 *       the following telemetry:
 *       - #MD_HkTlm_t.ValidCmdCntr will increment.
 *       - #MD_HkTlm_t.DwellEnabledMask will be enabled (bit 1 = TBL1, etc)
 *       - The #MD_START_DWELL_INF_EID informational event message
 *         will be issued.
 *
 *  \par Error Conditions
 *       This command may fail for the following reasons:
 *       - Unexpected command length.
 *       - Dwell Table mask argument contains no valid table values ( 1..#MD_NUM_DWELL_TABLES).
 *
 *       Evidence of an unexpected command length error may be found
 *       in the following telemetry:
 *       - #MD_HkTlm_t.InvalidCmdCntr increments.
 *       - The #MD_CMD_LEN_ERR_EID error event message is issued.
 *
 *       Evidence of an invalid value for Dwell Table mask argument may be found
 *       in the following telemetry:
 *       - #MD_HkTlm_t.InvalidCmdCntr increments.
 *       - The #MD_EMPTY_TBLMASK_ERR_EID error event message is issued.
 *
 **
 *  \par Criticality
 *       None.
 *
 *  \sa  #MD_STOP_DWELL_CC
 */
#define MD_START_DWELL_CC 2

/**
 * \brief Memory Dwell Stop Dwell Command
 *
 *  \par Description
 *      This command clears the Enabled flag(s) associated with the Dwell Table(s)
 *      that have been designated by the command's TableMask argument.
 *
 *      When the Enabled flag associated with a Dwell Table is cleared,
 *      dwell processing cannot occur for that Dwell Table.
 *
 *      Note that the value of the Enabled flag is also updated when a
 *      Dwell Table is loaded.
 *
 *      Note that the dwell state will not be affected for the Dwell Tables
 *      _not_ designated by the TableMask argument.  Thus, for example, if
 *      a Stop Dwell Command is issued to stop Dwell Table #2, all _other_
 *      Dwell Tables will remain in the same state following the command
 *      that they were in before the command was received.
 *
 *  \par Command Structure
 *       #MD_CmdStartStop_t
 *
 *  \par Command Verification
 *       Successful execution of this command may be verified with
 *       the following telemetry:
 *       - #MD_HkTlm_t.ValidCmdCntr increments.
 *       - #MD_HkTlm_t.DwellEnabledMask will be disabled (bit 1 = TBL1, etc)
 *       - The #MD_STOP_DWELL_INF_EID informational event message is issued.
 *
 *  \par Error Conditions
 *       This command may fail for the following reason(s):
 *       - Unexpected command length.
 *       - Dwell Table mask argument contains no valid table values ( 1..#MD_NUM_DWELL_TABLES).
 *
 *       Evidence of an unexpected command length error may be found
 *       in the following telemetry:
 *       - #MD_HkTlm_t.InvalidCmdCntr increments.
 *       - The #MD_CMD_LEN_ERR_EID error event message is issued.
 *
 *       Evidence of an invalid value for Dwell Table argument may be found
 *       in the following telemetry:
 *       - #MD_HkTlm_t.InvalidCmdCntr increments.
 *       - The #MD_EMPTY_TBLMASK_ERR_EID error event message is issued.
 *
 *  \par Criticality
 *       None.
 *
 *  \sa #MD_START_DWELL_CC
 */
#define MD_STOP_DWELL_CC 3

/**
 * \brief Jam Dwell
 *
 *  \par Description
 *      This command inserts the specified dwell parameters (dwell address,
 *      dwell field length, and delay count) into the specified table,
 *      at the specified index.
 *
 *      Note that it is safe to send a Jam command to an active Dwell Table.
 *      ('Active' indicates a Table which is enabled; thus, the Dwell Table
 *      is actively being used to generate a dwell packet telemetry stream.)
 *      Note that changes made to a Dwell Table using a Jam command will not
 *      be saved across process resets in this version of Memory Dwell.
 *
 *      For details on what constitutes a valid Dwell Table see #MD_DwellTableLoad_t.
 *      In particular, note that a valid entry _may_ be inserted past a terminator entry;
 *      however it won't be processed as long as it remains following a terminator entry.
 *
 *  \par Command Structure
 *       #MD_CmdJam_t
 *
 *  \par Command Verification
 *       Nominal successful execution of this command may be verified with
 *       the following telemetry:
 *       - #MD_HkTlm_t.ValidCmdCntr increments.
 *       - The #MD_JAM_DWELL_INF_EID or #MD_JAM_NULL_DWELL_INF_EID informational event message is issued.
 *
 *  \par Error Conditions
 *       This command may fail for the following reason(s):
 *       - Unexpected command length                    (Event message #MD_CMD_LEN_ERR_EID is issued)
 *       - Table Id other than 1..MD_NUM_DWELL_TABLES   (Event message #MD_INVALID_JAM_TABLE_ERR_EID is issued)
 *       - Entry Id other than 1..MD_DWELL_TABLE_SIZE   (Event message #MD_INVALID_ENTRY_ARG_ERR_EID is issued)
 *       - Unrecognized Dwell Address symbol            (Event message #MD_CANT_RESOLVE_JAM_ADDR_ERR_EID is issued)
 *       - Dwell Field Length other than 0, 1, 2, or 4  (Event message #MD_INVALID_LEN_ARG_ERR_EID is issued)
 *       - Specified Dwell Address is out of range      (Event message #MD_INVALID_JAM_ADDR_ERR_EID is issued)
 *       - Specified Dwell Address is not properly aligned for the specified Dwell Length
 *         (Event message #MD_JAM_ADDR_NOT_32BIT_ERR_EID or #MD_JAM_ADDR_NOT_16BIT_ERR_EID is issued)
 *
 *       Any time the command fails, #MD_HkTlm_t.InvalidCmdCntr increments.
 *
 *  \par Criticality
 *       None.
 */
#define MD_JAM_DWELL_CC 4

#if MD_SIGNATURE_OPTION == 1

/**
 * \brief Set Signature Command
 *
 *  \par Description
 *       Associates a signature with the specified Dwell Table.
 *
 *  \par Command Structure
 *       #MD_CmdSetSignature_t
 *
 *  \par Command Verification
 *       Successful execution of this command may be verified with
 *       the following telemetry:
 *       - #MD_HkTlm_t.ValidCmdCntr will increment.
 *
 *  \par Error Conditions
 *       This command may fail for the following reason(s):
 *       - Unexpected command length.                    (Event message #MD_CMD_LEN_ERR_EID is issued)
 *       - Dwell Table ID is invalid.                    (Event message #MD_INVALID_SIGNATURE_TABLE_ERR_EID is issued)
 *
 *       Any time the command fails, #MD_HkTlm_t.InvalidCmdCntr increments.
 *
 *  \par Criticality
 *       None.
 */
#define MD_SET_SIGNATURE_CC 5

#endif

/**\}*/

#endif
