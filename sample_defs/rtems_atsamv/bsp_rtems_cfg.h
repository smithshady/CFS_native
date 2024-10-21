/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as �~@~\core Flight System: Bootes�~@~]
 *
 * Copyright (c) 2020 United States Government as represented by the
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
 *   RTEMS OS Configuration definitions
 *
 * @note
 *   This file overrides:
 *   	osal/src/bsp/generic-rtems/config/default_bsp_rtems_cfg.h
 */
#ifndef BSP_RTEMS_CFG_H
#define BSP_RTEMS_CFG_H

#include <libchip/chip.h>

/* clock configuration -- 408/204/102 for PLLA/HCLK/MCK */
const struct atsam_clock_config atsam_clock_config = {
  .pllar_init = (CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(0x21U) |
      CKGR_PLLAR_PLLACOUNT(0x3FU) | CKGR_PLLAR_DIVA(0x1U)),
  .mckr_init = (PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK |
      PMC_MCKR_MDIV_PCK_DIV2),
  .mck_freq = 102*1000*1000
};

#include "osconfig.h"

//#define CONFIGURE_VERBOSE_SYSTEM_INITIALIZATION

#define TASK_INTLEVEL 0
#define CONFIGURE_INIT
#define CONFIGURE_INIT_TASK_ATTRIBUTES \
    (RTEMS_FLOATING_POINT | RTEMS_PREEMPT | RTEMS_NO_TIMESLICE | RTEMS_ASR | RTEMS_INTERRUPT_LEVEL(TASK_INTLEVEL))
#define CONFIGURE_INIT_TASK_STACK_SIZE (20 * 1024)
#define CONFIGURE_INIT_TASK_PRIORITY   10

/*
 * Note that these resources are shared with RTEMS itself (e.g. the init task, the shell)
 * so they should be allocated slightly higher than the user limits in osconfig.h
 *
 * Many RTEMS services use tasks internally, including the idle task, BSWP, ATA driver,
 * low level console I/O, the shell, TCP/IP network stack, and DHCP (if enabled).
 * Many of these also use semaphores for synchronization.
 *
 * Budgeting for additional:
 *   8 internal tasks
 *   2 internal timers
 *   4 internal queues
 *   16 internal semaphores
 *
 */
#define CONFIGURE_MAXIMUM_TASKS          (OS_MAX_TASKS + 8)
#define CONFIGURE_MAXIMUM_TIMERS         (OS_MAX_TIMERS + 2)
#define CONFIGURE_MAXIMUM_SEMAPHORES     (OS_MAX_BIN_SEMAPHORES + OS_MAX_COUNT_SEMAPHORES + OS_MAX_MUTEXES + 16)
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES (OS_MAX_QUEUES + 4)
#define CONFIGURE_MAXIMUM_DRIVERS        10
#define CONFIGURE_MAXIMUM_POSIX_KEYS     4
#ifdef OS_RTEMS_4_DEPRECATED
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS (OS_MAX_NUM_OPEN_FILES + 8)
#else
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS (OS_MAX_NUM_OPEN_FILES + 8)
#endif

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_FILESYSTEM_RFS
#define CONFIGURE_FILESYSTEM_IMFS
#define CONFIGURE_FILESYSTEM_DOSFS
#define CONFIGURE_FILESYSTEM_DEVFS
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK
/* these are out of date and should not be used! */
/* see: https://docs.rtems.org/branches/master/bsp-howto/ide_controller.html */
//#define CONFIGURE_APPLICATION_NEEDS_IDE_DRIVER
//#define CONFIGURE_APPLICATION_NEEDS_ATA_DRIVER

#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS					/* single memory pool for WS and heap */
//#define CONFIGURE_MEMORY_OVERHEAD		512			/* KiB */
//#define CONFIGURE_EXECUTIVE_RAM_SIZE		(4 * 1024 * 1024)
#define CONFIGURE_MICROSECONDS_PER_TICK		10000			/* 1 tick == 10 ms */
//#define CONFIGURE_ATA_DRIVER_TASK_PRIORITY	9

#endif
