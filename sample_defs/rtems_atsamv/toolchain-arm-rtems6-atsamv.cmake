# Note that to use this, the "RTEMS" platform module may need to be added
# to the system-wide CMake installation as a default CMake does not yet
# recognize RTEMS as a system name.  An example of this is distributed with
# the pc-rtems PSP.

# Basic cross system configuration
set(CMAKE_SYSTEM_NAME       RTEMS)
set(CMAKE_SYSTEM_PROCESSOR  arm)
set(CMAKE_SYSTEM_VERSION    6)

# The RTEMS BSP that will be used for this build
set(RTEMS_BSP               "atsamv")

# these settings are specific to cFE/OSAL and determines which
# abstraction layers are built when using this toolchain
SET(CFE_SYSTEM_PSPNAME      generic-rtems)
SET(OSAL_SYSTEM_BSPTYPE     generic-rtems)
SET(OSAL_SYSTEM_OSTYPE      rtems)

# This is for version specific RTEMS ifdefs needed by the OSAL and PSP
ADD_DEFINITIONS(-DOS_RTEMS_6)

# OSAL fails to build with the bsp_cmdline implementation
set(RTEMS_NO_CMDLINE TRUE)
set(RTEMS_NO_SHELL TRUE)

# Tars object/table/script files, attaches to executable, untars into IMFS on startup
set(RTEMS_INCLUDE_TARFS TRUE)

# RTEMS_DYNAMIC_LOAD definition:
# - Set to FALSE for platforms that create a RTEMS executable and link it
#   to the cFE core.
# - Set to TRUE for platforms that expect the cFE core to to be dynamically
#   loaded into an existing runtime image.
# This is tied to the OSAL-BSP and PSP implementation so generally cannot
# be switched on a specific OSAL/PSP platform without modifications.
set(RTEMS_DYNAMIC_LOAD      FALSE)

set(RTEMS_BSP_C_FLAGS       "-mthumb -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mno-unaligned-access -Wl,--gc-sections -qnolinkcmds -Wl,-Tlinkcmds.extram")
set(RTEMS_BSP_CXX_FLAGS     ${RTEMS_BSP_C_FLAGS})
set(RTEMS_BSP_SPECS_FLAGS   "")

# Info regarding the RELOCADDR:
#+--------------------------------------------------------------------------+
#| Set the value of RELOCADDR to the address where you want your image to
#| load. If you'll be using GRUB to load the images it will have to be >=
#| 0x100000 (1024K). If you are using NetBoot to load the images it can be
#| >= 0x10000 (64K) AND <= 0x97C00 (607K) OR >= 0x100000 (1024K). The memory
#| top is of course another limit. Make sure there is enough space before the
#| upper memory limits for the image and the memory allocated by it to fit.
#| Make sure the value you choose is aligned to 4 bytes.
#+--------------------------------------------------------------------------+
#set(RTEMS_RELOCADDR 0x00400000) # WHY is this here???

# Exception handling is very iffy.  These two options disable eh_frame creation.
set(CMAKE_C_COMPILE_OPTIONS_PIC -fno-exceptions -fno-asynchronous-unwind-tables)

# Link libraries needed for a RTEMS 5+ executable
#  This was handled by the bsp_specs file in 4.11
set(LINK_LIBRARIES              "-lrtemsdefaultconfig -lrtemsbsp -lrtemscpu -lnetworking")

#+---------------------------------------------------------------------------+ 
#| Common RTEMS toolchain statements
#+---------------------------------------------------------------------------+ 
# The TOOLS and BSP are allowed to be installed in different locations.
# If the README was followed they will both be installed under $HOME
# By default it is assumed the BSP is installed to the same directory as the tools
SET(RTEMS_TOOLS_PREFIX "/opt/rtems/${CMAKE_SYSTEM_VERSION}" CACHE PATH
    "RTEMS tools install directory")
SET(RTEMS_BSP_PREFIX "${RTEMS_TOOLS_PREFIX}" CACHE PATH
    "RTEMS BSP install directory")

# specify the cross compiler - adjust accord to compiler installation
SET(SDKHOSTBINDIR               "${RTEMS_TOOLS_PREFIX}/bin")
set(TARGETPREFIX                "${CMAKE_SYSTEM_PROCESSOR}-rtems${CMAKE_SYSTEM_VERSION}-")

SET(CMAKE_C_COMPILER            "${RTEMS_TOOLS_PREFIX}/bin/${TARGETPREFIX}gcc")
SET(CMAKE_CXX_COMPILER          "${RTEMS_TOOLS_PREFIX}/bin/${TARGETPREFIX}g++")
SET(CMAKE_LINKER                "${RTEMS_TOOLS_PREFIX}/bin/${TARGETPREFIX}ld")
SET(CMAKE_ASM_COMPILER          "${RTEMS_TOOLS_PREFIX}/bin/${TARGETPREFIX}as")
SET(CMAKE_STRIP                 "${RTEMS_TOOLS_PREFIX}/bin/${TARGETPREFIX}strip")
SET(CMAKE_NM                    "${RTEMS_TOOLS_PREFIX}/bin/${TARGETPREFIX}nm")
SET(CMAKE_AR                    "${RTEMS_TOOLS_PREFIX}/bin/${TARGETPREFIX}ar")
SET(CMAKE_OBJDUMP               "${RTEMS_TOOLS_PREFIX}/bin/${TARGETPREFIX}objdump")
SET(CMAKE_OBJCOPY               "${RTEMS_TOOLS_PREFIX}/bin/${TARGETPREFIX}objcopy")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM   NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY   ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE   ONLY)

SET(CMAKE_PREFIX_PATH                   /)
