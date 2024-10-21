/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "definitions.h"
#include "device.h"
#include "watchdog/header/watchdog.h"
#include "memory/header/memory.h"
#include "power/header/power_driver.h"
#include "power/header/power_driver_controller.h"
#include "thermal/driver/header/thermal_driver_api.h"
#include "thermal/controller/header/thermal_controller_api.h"
#include "motors/driver/header/motors_driver.h"
#include "motors/controller/header/motors_velocity_controller_api.h"
#include "localization/imu/header/imu_driver.h"
#include "localization/localizer/header/localizer.h"
#include "drive_controller/header/drive_controller.h"
#include "payloads/thermal_switch/header/thermal_switch_api.h"
#include "ftfc_logger/logger.h"
#include "logger/header/logger_functions.h"
#include "camera/common/header/camera_api.h"
#include "system_state_machine/state_machine/header/system_state_machine.h"
#include "fpga/driver/header/fpga_driver_api.h"
#include "communication/antenna/header/antenna_api.h"
#include "communication/interfaces/micro_hard_radio/header/micro_hard_radio_api.h"
#include "health/header/health_comms_api.h"
#include "hdrm/header/hdrm_api.h"

#include "communication/comms/comms_receive/header/comms_receive_api.h"
#include "communication/comms/comms_backfill/header/comms_backfill_api.h"

#include "communication/ping/header/comms_ping.h"

#include "mission_configuration_definitions.h"

// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
#pragma config TCM_CONFIGURATION = 0
#pragma config SECURITY_BIT = CLEAR
#pragma config BOOT_MODE = SET

// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Logger Functions
// *****************************************************************************
// *****************************************************************************

/* Structure to hold pointers to project specific logger functions */
LOGGER_Functions log_memory_functions = {
    .ReadConfig = LOGGER_ReadConfig,
    .WriteConfig = LOGGER_WriteConfig,
    .SendMessage = LOGGER_SendMessage
};

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="SYS_TIME Initialization Data">

const SYS_TIME_PLIB_INTERFACE sysTimePlibAPI = {
    .timerCallbackSet = (SYS_TIME_PLIB_CALLBACK_REGISTER)TC3_CH0_TimerCallbackRegister,
    .timerStart = (SYS_TIME_PLIB_START)TC3_CH0_TimerStart,
    .timerStop = (SYS_TIME_PLIB_STOP)TC3_CH0_TimerStop ,
    .timerFrequencyGet = (SYS_TIME_PLIB_FREQUENCY_GET)TC3_CH0_TimerFrequencyGet,
    .timerPeriodSet = (SYS_TIME_PLIB_PERIOD_SET)TC3_CH0_TimerPeriodSet,
    .timerCompareSet = (SYS_TIME_PLIB_COMPARE_SET)TC3_CH0_TimerCompareSet,
    .timerCounterGet = (SYS_TIME_PLIB_COUNTER_GET)TC3_CH0_TimerCounterGet,
};

const SYS_TIME_INIT sysTimeInitData =
{
    .timePlib = &sysTimePlibAPI,
    .hwTimerIntNum = TC3_CH0_IRQn,
};

// </editor-fold>



// *****************************************************************************
// *****************************************************************************
// Section: Local initialization functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void STDIO_BufferModeSet ( void )

  Summary:
    Sets the buffering mode for stdin and stdout

  Remarks:
 ********************************************************************************/
static void STDIO_BufferModeSet(void)
{

    /* Make stdin unbuffered */
    setbuf(stdin, NULL);

    /* Make stdout unbuffered */
    setbuf(stdout, NULL);
}


/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{
    /* Hardware peripherals init */
    EFC_Initialize();
    STDIO_BufferModeSet();
    CLOCK_Initialize();
    PIO_Initialize();
    RSWDT_Initialize();
    WDT_Initialize();
    PWM0_Initialize();
    TC3_CH0_TimerInitialize();
    UART0_Initialize();
    RSTC_Initialize();
    UART1_Initialize();
    UART3_Initialize();
    SPI0_Initialize();
    MCAN1_Initialize();
    SPI1_Initialize();
    USART1_Initialize();
    USART0_Initialize();
    NVIC_Initialize();

    /* Level-0 Task creation */
    WATCHDOG_Initialize();
    LOGGER_Initialize(&log_memory_functions);
    LOGGER_PrintMissionConfiguration();

    COMMS_TRANSMIT_API_Initialize();
    COMMS_RECEIVE_API_Initialize();
    COMMS_BACKFILL_API_Initialize();
    
    /* Level-1 task creation */
    FPGA_DRIVER_API_Initialize(); /* Needed by thermal and memory */
    MEMORY_Initialize(); /* Needed by all configurable modules */
    POWER_DRIVER_Initialize(); /* Needed by power controller */
    POWER_DRIVER_CONTROLLER_Initialize(); /* Needed to power peripherals */

    /* Thermal Control System */
    THERMAL_DRIVER_API_Initialize(); /* Needed to interface with TCS HW */
    THERMAL_CONTROLLER_API_Initialize(); /* Automatic HEATR control */
    THERMAL_SWITCH_API_Initialize(); /* Support Thermal Switch */
    
    /* Optical Stack */
    CAMERA_API_Initialize();
#if MISSION_NUMBER == MISSION_TWO
    HDRM_API_Initialize();
#endif

    /* Localization and Drive Control Stack */
    MOTORS_DRIVER_Initialize(); /* Needed by localizer */
    MOTORS_VELOCITY_CONTROLLER_API_Initialize(); /* Needed for vel. control */
    IMU_DRIVER_Initialize();
    LOCALIZER_Initialize(); /* Needed by drive controller */
    DRIVE_CONTROLLER_Initialize();

    /* Antenna Actuation Support */
    ANTENNA_API_Initialize();

    /* Communicate systems online */
    COMMS_PING_Initialize();
    (void) HEALTH_COMMS_API_Initialize();

    /* Start Main Program */
    SYSTEM_STATE_MACHINE_Initialize();

}


/*******************************************************************************
 End of File
*/
