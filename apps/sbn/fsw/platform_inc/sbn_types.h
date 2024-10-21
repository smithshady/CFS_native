/******************************************************************************
** File: sbn_types.h
**
**      Copyright (c) 2004-2016, United States government as represented by the
**      administrator of the National Aeronautics Space Administration.
**      All rights reserved. This software(cFE) was created at NASA's Goddard
**      Space Flight Center pursuant to government contracts.
**
**      This software may be used only pursuant to a United States government
**      sponsored project and the United States government may not be charged
**      for use thereof.
**
** Purpose:
**      This header file contains prototypes for private functions and type
**      definitions for the Software Bus Network Application.
**
** Authors:   J. Wilmot/GSFC Code582
**            R. McGraw/SSI
**            C. Knight/ARC Code TI
******************************************************************************/
#include "cfe.h"

#ifndef _sbn_types_h_
#define _sbn_types_h_

/**
 * Below are types that the user shouldn't have to change, but are useful
 * to know and are shared with modules.
 * If it's a compile-time configuration option, it should be in
 * sbn_platform_cfg.h instead.
 */

typedef enum
{
    SBN_SUCCESS = 0,
    SBN_ERROR,
    SBN_IF_EMPTY,
    SBN_NOT_IMPLEMENTED
} SBN_Status_t;

typedef enum
{
    SBN_TASK_POLL = 0x00, /**< @brief poll connections for this net/peer */
    SBN_TASK_SEND = 0x01, /**< @brief create a task for each net/peer and blocks on the pipe */
    SBN_TASK_RECV = 0x02, /**< @brief create a task for each net/peer and blocks on the net recv */
    SBN_TASKS     = SBN_TASK_SEND | SBN_TASK_RECV, /**< @brief create two tasks per net/peer, tasks block on reads */
} SBN_Task_Flag_t;

typedef enum
{
    SBN_UDP            = 1,
    SBN_TCP            = 2,
    SBN_SPACEWIRE_RMAP = 3,
    SBN_SPACEWIRE_PKT  = 4,
    SBN_SHMEM          = 5,
    SBN_SERIAL         = 6,
    SBN_1553           = 7,
    SBN_DTN            = 8,
} SBN_Protocol_t;

/**
 * Message types definitions, reserve the high 128 values for module-specific
 * message types.
 */
typedef enum
{
    SBN_NO_MSG    = 0x00, /**< @brief no payload */
    SBN_SUB_MSG   = 0x01, /**< @brief payload is subs */
    SBN_UNSUB_MSG = 0x02, /**< @brief payload is unsubs */
    SBN_APP_MSG   = 0x03, /**< @brief payload is SB msg */
    SBN_PROTO_MSG = 0x04, /**< @brief payload is SBN proto */
} SBN_MsgTypeEnum_t;

/**
 * Mask to identift module-specific message types
 */
#define SBN_MODULE_SPECIFIC_MESSAGE_ID_MASK (0x80)

/**
 * @brief Generated by GIT. Used to ensure peers are running the same
 * version of code.
 */
#define SBN_IDENT "$Id$"

/** @brief Id is always the same len, plus \0 */
#define SBN_IDENT_LEN 48

#define SBN_PROTO_VER 11

/* used in local and peer subscription tables */
typedef struct
{
    uint32         InUseCtr;
    CFE_SB_MsgId_t MsgID;
    CFE_SB_Qos_t   QoS;
} SBN_Subs_t;

/* most/all scalars should be typedef'd for readability and type checking */
typedef int16             SBN_MsgSz_t; /* needs to support < 0 for errs */
typedef uint8             SBN_MsgType_t;
typedef uint8             SBN_ModuleIdx_t;
typedef uint8             SBN_NetIdx_t;
typedef uint16            SBN_PeerIdx_t;
typedef uint32            CFE_ProcessorID_t;
typedef uint32            CFE_SpacecraftID_t;
typedef uint32            OS_TaskID_t;
typedef uint32            OS_MutexID_t;
typedef int32             OS_SocketPort_t;
typedef uint32            OS_SocketID_t;
typedef int32             OS_FileDes_t;
typedef uint32            OS_SockFileDes_t;
typedef int32             OS_SelectTimeout_t;
typedef int32             OS_Status_t;
typedef uint32            CFE_ES_AppID_t;
typedef uint32            CFE_ES_ObjectID_t;
typedef CFE_ES_ObjectID_t CFE_ES_ModuleID_t;
typedef CFE_ES_ObjectID_t CFE_ES_MutexID_t;
typedef uint16            CFE_EVS_EventID_t;
typedef int16             SBN_SubCnt_t;
typedef uint16            SBN_HKTlm_t;

#define EVSSendInfo(E, ...) CFE_EVS_SendEvent((E), CFE_EVS_EventType_INFORMATION, __VA_ARGS__)
#define EVSSendDbg(E, ...)  CFE_EVS_SendEvent((E), CFE_EVS_EventType_DEBUG, __VA_ARGS__)
#define EVSSendErr(E, ...)  CFE_EVS_SendEvent((E), CFE_EVS_EventType_ERROR, __VA_ARGS__)
#define EVSSendCrit(E, ...) CFE_EVS_SendEvent((E), CFE_EVS_EventType_CRITICAL, __VA_ARGS__)

/*****************************************************************************/
#endif /* _sbn_types_h_ */
