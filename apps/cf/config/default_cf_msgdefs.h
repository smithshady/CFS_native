/************************************************************************
 * NASA Docket No. GSC-18,447-1, and identified as “CFS CFDP (CF)
 * Application version 3.0.0”
 *
 * Copyright (c) 2019 United States Government as represented by the
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
 *   Specification for the CFS CFDP (CF) command and telemetry
 *   message payload content definitions.
 */
#ifndef CF_MSGDEFS_H
#define CF_MSGDEFS_H

#include "common_types.h"
#include "cf_extern_typedefs.h"

/**
 * \defgroup cfscftlm CFS CFDP Telemetry
 * \{
 */

/**
 * \brief Housekeeping command counters
 */
typedef struct CF_HkCmdCounters
{
    uint16 cmd; /**< \brief Command success counter */
    uint16 err; /**< \brief Command error counter */
} CF_HkCmdCounters_t;

/**
 * \brief Housekeeping sent counters
 */
typedef struct CF_HkSent
{
    uint64 file_data_bytes;      /**< \brief Sent File data bytes */
    uint32 pdu;                  /**< \brief Sent PDUs counter */
    uint32 nak_segment_requests; /**< \brief Sent NAK segment requests counter */
} CF_HkSent_t;

/**
 * \brief Housekeeping received counters
 */
typedef struct CF_HkRecv
{
    uint64 file_data_bytes;      /**< \brief Received File data bytes */
    uint32 pdu;                  /**< \brief Received PDUs with valid header counter */
    uint32 error;                /**< \brief Received PDUs with error counter, see related event for cause */
    uint16 spurious;             /**< \brief Received PDUs with invalid directive code for current context or
                                  *          file directive FIN without matching active transaction counter,
                                  *          see related event for cause
                                  */
    uint16 dropped;              /**< \brief Received PDUs dropped due to a transaction error */
    uint32 nak_segment_requests; /**< \brief Received NAK segment requests counter */
} CF_HkRecv_t;

/**
 * \brief Housekeeping fault counters
 */
typedef struct CF_HkFault
{
    uint16 file_open;          /**< \brief File open fault counter */
    uint16 file_read;          /**< \brief File read fault counter */
    uint16 file_seek;          /**< \brief File seek fault counter */
    uint16 file_write;         /**< \brief File write fault counter */
    uint16 file_rename;        /**< \brief File rename fault counter */
    uint16 directory_read;     /**< \brief Directory read fault counter */
    uint16 crc_mismatch;       /**< \brief CRC mismatch fault counter */
    uint16 file_size_mismatch; /**< \brief File size mismatch fault counter */
    uint16 nak_limit;          /**< \brief NAK limit exceeded fault counter */
    uint16 ack_limit;          /**< \brief ACK limit exceeded fault counter */
    uint16 inactivity_timer;   /**< \brief Inactivity timer exceeded counter */
    uint16 spare;              /**< \brief Alignment spare to avoid implicit padding */
} CF_HkFault_t;

/**
 * \brief Housekeeping counters
 */
typedef struct CF_HkCounters
{
    CF_HkSent_t  sent;  /**< \brief Sent counters */
    CF_HkRecv_t  recv;  /**< \brief Received counters */
    CF_HkFault_t fault; /**< \brief Fault counters */
} CF_HkCounters_t;

/**
 * \brief Housekeeping channel data
 */
typedef struct CF_HkChannel_Data
{
    CF_HkCounters_t counters;                /**< \brief Counters */
    uint16          q_size[CF_QueueIdx_NUM]; /**< \brief Queue sizes */
    uint8           poll_counter;            /**< \brief Number of active polling directories */
    uint8           playback_counter;        /**< \brief Number of active playback directories */
    uint8           frozen;                  /**< \brief Frozen state: 0 == not frozen, else frozen */
    uint8           spare[7];                /**< \brief Alignment spare (uint64 values in the counters) */
} CF_HkChannel_Data_t;

/**
 * \brief Housekeeping packet
 */
typedef struct CF_HkPacket_Payload
{
    CF_HkCmdCounters_t counters; /**< \brief Command counters */
    uint8              spare[4]; /**< \brief Alignment spare (CF_HkCmdCounters_t is 4 bytes) */

    CF_HkChannel_Data_t channel_hk[CF_NUM_CHANNELS]; /**< \brief Per channel housekeeping data */
} CF_HkPacket_Payload_t;

/**
 * \brief End of transaction packet
 */
typedef struct CF_EotPacket_Payload
{
    CF_TransactionSeq_t seq_num;    /**< \brief transaction identifier, stays constant for entire transfer */
    uint32              channel;    /**< \brief Channel number */
    uint32              direction;  /**< \brief direction of this transaction */
    uint32              state;      /**< \brief Transaction state */
    uint32              txn_stat;   /**< \brief final status code of transaction (extended CFDP CC) */
    CF_EntityId_t       src_eid;    /**< \brief the source eid of the transaction */
    CF_EntityId_t       peer_eid;   /**< \brief peer_eid is always the "other guy", same src_eid for RX */
    uint32              fsize;      /**< \brief File size */
    uint32              crc_result; /**< \brief CRC result */
    CF_TxnFilenames_t   fnames;     /**< \brief file names associated with this transaction */
} CF_EotPacket_Payload_t;

/**\}*/

/**
 * \defgroup cfscfcmdstructs CFS CFDP Command Structures
 * \{
 */

/**
 * \brief Command payload argument union to support 4 uint8's, 2 uint16's or 1 uint32
 */
typedef union CF_UnionArgs_Payload
{
    uint32 dword;    /**< \brief Generic uint32 argument */
    uint16 hword[2]; /**< \brief Generic uint16 array of arguments */
    uint8  byte[4];  /**< \brief Generic uint8 array of arguments */
} CF_UnionArgs_Payload_t;

/**
 * \brief IDs for use for Reset cmd
 */
typedef enum
{
    CF_Reset_all     = 0, /**< \brief Reset all */
    CF_Reset_command = 1, /**< \brief Reset command */
    CF_Reset_fault   = 2, /**< \brief Reset fault */
    CF_Reset_up      = 3, /**< \brief Reset up */
    CF_Reset_down    = 4  /**< \brief Reset down */
} CF_Reset_t;

/**
 * \brief Type IDs for use for Write Queue cmd
 */
typedef enum
{
    CF_Type_all  = 0, /**< \brief Type all */
    CF_Type_up   = 1, /**< \brief Type up */
    CF_Type_down = 2  /**< \brief Type down */
} CF_Type_t;

/**
 * \brief Queue IDs for use for Write Queue cmd
 */
typedef enum
{
    CF_Queue_pend    = 0, /**< \brief Queue pending */
    CF_Queue_active  = 1, /**< \brief Queue active */
    CF_Queue_history = 2, /**< \brief Queue history */
    CF_Queue_all     = 3  /**< \brief Queue all */
} CF_Queue_t;

/**
 * \brief Parameter IDs for use with Get/Set parameter messages
 *
 * Specifically these are used for the "key" field within CF_GetParamCmd_t and
 * CF_SetParamCmd_t message structures.
 */
typedef enum
{
    CF_GetSet_ValueID_ticks_per_second,                      /**< \brief Ticks per second key */
    CF_GetSet_ValueID_rx_crc_calc_bytes_per_wakeup,          /**< \brief Receive CRC calculated bytes per wake-up key */
    CF_GetSet_ValueID_ack_timer_s,                           /**< \brief ACK timer in seconds key */
    CF_GetSet_ValueID_nak_timer_s,                           /**< \brief NAK timer in seconds key */
    CF_GetSet_ValueID_inactivity_timer_s,                    /**< \brief Inactivity timer in seconds key */
    CF_GetSet_ValueID_outgoing_file_chunk_size,              /**< \brief Outgoing file chunk size key */
    CF_GetSet_ValueID_ack_limit,                             /**< \brief ACK retry limit key */
    CF_GetSet_ValueID_nak_limit,                             /**< \brief NAK retry limit key */
    CF_GetSet_ValueID_local_eid,                             /**< \brief Local entity id key */
    CF_GetSet_ValueID_chan_max_outgoing_messages_per_wakeup, /**< \brief Max outgoing messages per wake-up key */
    CF_GetSet_ValueID_MAX                                    /**< \brief Key limit used for validity check */
} CF_GetSet_ValueID_t;

/**
 * \brief Get parameter command structure
 *
 * For command details see #CF_GET_PARAM_CC
 */
typedef struct CF_GetParam_Payload
{
    uint8 key;      /**< \brief Parameter key, see #CF_GetSet_ValueID_t */
    uint8 chan_num; /**< \brief Channel number */
} CF_GetParam_Payload_t;

/**
 * \brief Set parameter command structure
 *
 * For command details see #CF_SET_PARAM_CC
 */
typedef struct CF_SetParam_Payload
{
    uint32 value;    /**< \brief Parameter value to set */
    uint8  key;      /**< \brief Parameter key, see #CF_GetSet_ValueID_t */
    uint8  chan_num; /**< \brief Channel number */
    uint8  spare[2]; /**< \brief Alignment spare, uint32 multiple */
} CF_SetParam_Payload_t;

/**
 * \brief Transmit file command structure
 *
 * For command details see #CF_TX_FILE_CC
 */
typedef struct CF_TxFile_Payload
{
    uint8         cfdp_class;                        /**< \brief CFDP class: 0=class 1, 1=class 2 */
    uint8         keep;                              /**< \brief Keep file flag: 1=keep, else delete */
    uint8         chan_num;                          /**< \brief Channel number */
    uint8         priority;                          /**< \brief Priority: 0=highest priority */
    CF_EntityId_t dest_id;                           /**< \brief Destination entity id */
    char          src_filename[CF_FILENAME_MAX_LEN]; /**< \brief Source file/directory name */
    char          dst_filename[CF_FILENAME_MAX_LEN]; /**< \brief Destination file/directory name */
} CF_TxFile_Payload_t;

/**
 * \brief Write Queue command structure
 *
 * For command details see #CF_WRITE_QUEUE_CC
 */
typedef struct CF_WriteQueue_Payload
{
    uint8 type;  /**< \brief Transaction direction: all=0, up=1, down=2 */
    uint8 chan;  /**< \brief Channel number */
    uint8 queue; /**< \brief Queue type: 0=pending, 1=active, 2=history, 3=all */
    uint8 spare; /**< \brief Alignment spare, puts filename on 32-bit boundary */

    char filename[CF_FILENAME_MAX_LEN]; /**< \brief Filename written to */
} CF_WriteQueue_Payload_t;

/**
 * \brief Transaction command structure
 *
 * For command details see #CF_SUSPEND_CC, #CF_RESUME_CC, #CF_CANCEL_CC, #CF_ABANDON_CC
 */
typedef struct CF_Transaction_Payload
{
    CF_TransactionSeq_t ts;       /**< \brief Transaction sequence number */
    CF_EntityId_t       eid;      /**< \brief Entity id */
    uint8               chan;     /**< \brief Channel number: 254=use ts, 255=all channels, else channel */
    uint8               spare[3]; /**< \brief Alignment spare for 32-bit multiple */
} CF_Transaction_Payload_t;

/**\}*/

#endif
