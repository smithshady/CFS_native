/******************************************************************************/
/** \file  MISSION_to_types.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Command and telemetry data strucutres for TO application (RS422)
*
*   \par
*       This header file contains definitions of command and telemetry data
*       structures for TO applications for the UDP transport protocol example.
*
*   \par Limitations, Assumptions, External Events, and Notes:
*     - Make use of the setup.sh script to move / link this file to the
*     {MISSION_HOME}/apps/inc/ folder.
*     - Standard command messages are defined in to_cmds.h
*     - Default HK Telemetry structure is defined in to_hktlm.h
*
*   \par Modification History:
*     - 2015-01-09 | Guy de Carufel | Code Started
*     - 2015-09-22 | Guy de Carufel | Moved hktlm to to_hktlm.h
*******************************************************************************/
#ifndef _MISSION_TO_TYPES_H_
#define _MISSION_TO_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "cfe.h"
#include "../to/fsw/src/to_hktlm.h"

/*
** Defines
*/

/* Define enable / disable commands */
typedef struct
{
   CFE_MSG_CommandHeader_t	ucCmdHeader;
   int32    iFileDesc;       /**< File Descriptor of Port to use. */
} TO_EnableOutputCmd_t;


typedef struct
{
   CFE_MSG_CommandHeader_t	ucCmdHeader;
} TO_DisableOutputCmd_t;



/*************** Telemetry **************/
typedef struct
{
    CFE_MSG_TelemetryHeader_t  ucTlmHeader;
} TO_InData_t;

typedef struct
{
    CFE_MSG_TelemetryHeader_t  ucTlmHeader;
    uint32  uiCounter;
} TO_OutData_t;


#ifdef __cplusplus
}
#endif

#endif /* _MISSION_TO_TYPES_H_ */

/*==============================================================================
** End of file MISSION_to_types.h
**============================================================================*/
