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
 *  The CF Application default configuration table
 */
#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "cf_tbldefs.h"
#include "cf_msgids.h"

CF_ConfigTable_t CF_config_table = {
    10,    /* ticks_per_second */
    16384, /* max number of bytes per wakeup to calculate r2 recv file CRC */
    25,    /* local entity id */
    {      /* channel configuration for CF_NUM_CHANNELS */
     {
         /* channel 0 */
         5,      /* max number of outgoing messages per wakeup */
         5,      /* max number of rx messages per wakeup */
         3,      /* ACK timer */
         3,      /* NAK timer */
         30,     /* inactivity timer */
         4,      /* ACK limit */
         4,      /* NAK limit */
         CF_CHANNEL_0_INPUT_MID, /* input message id */
         CF_CHANNEL_0_OUTPUT_MID, /* output message id */
         16,     /* input pipe depth */
         {       /* polling directory configuration for CF_MAX_POLLING_DIR_PER_CHAN */
          {
              /* polling directory 0 */
              5,               /* interval seconds */
              25,              /* priority */
              CF_CFDP_CLASS_1, /* class to send */
              2,              /* destination entity id */
              "/ram",  /* source directory */
              "/",    /* destination directory */
              0                /* polling directory enable flag (1 = enabled) */
          },
          {
              0 /* zero fill unused polling directory slots */
          }},
         "",            /* throttle sem, empty string means no throttle */
         1,             /* dequeue enable flag (1 = enabled) */
         .move_dir = "" /* If not empty, will attempt move instead of delete on TX file complete */
     },
     {        /* channel 1 */
      5,      /* max number of outgoing messages per wakeup */
      5,      /* max number of rx messages per wakeup */
      3,      /* ack timer */
      3,      /* nak timer */
      30,     /* inactivity timer */
      4,      /* ack limit */
      4,      /* nak limit */
      0x18c9, /* input message id */
      0x08c3, /* output message id */
      16,     /* input pipe depth */
      {       /* polling directory configuration for CF_MAX_POOLING_DIR_PER_CHAN */
       {
           0 /* zero fill unused polling directory slots */
       }},
      "", /* throttle sem, empty string means no throttle */
      1,  /* dequeue enable flag (1 = enabled) */
      .move_dir = ""}},
    480,       /* outgoing_file_chunk_size */
    "/cf/tmp", /* temporary file directory */
    "/cf/fail", /* Stores failed tx file for "polling directory" */
};
CFE_TBL_FILEDEF(CF_config_table, CF.config_table, CF config table, cf_def_config.tbl)
