include("${CMAKE_CURRENT_LIST_DIR}/find_compiler.cmake")

set(CMAKE_SYSTEM_NAME xc32-samx7x)

# Find Microchip XC32.
xc32_find_compiler(XC32_COMPILER_CC xc32-gcc)
xc32_find_compiler(XC32_COMPILER_CXX xc32-g++)
set(XC32_COMPILER_ASM "${XC32_COMPILER_CC}" CACHE INTERNAL "")

# Specify the cross compiler.
set(CMAKE_C_COMPILER ${XC32_COMPILER_CC} CACHE FILEPATH "C compiler")
set(CMAKE_CXX_COMPILER ${XC32_COMPILER_CXX} CACHE FILEPATH "C++ compiler")
set(CMAKE_ASM_COMPILER ${XC32_COMPILER_ASM} CACHE FILEPATH "ASM compiler")

# Disable compiler checks.
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

# Add target system root to cmake find path.
get_filename_component(XC32_COMPILER_DIR "${XC32_COMPILER_CC}" DIRECTORY)
get_filename_component(CMAKE_FIND_ROOT_PATH "${XC32_COMPILER_DIR}" DIRECTORY)

# Don't look for executable in target system prefix.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Look for includes and libraries only in the target system prefix.
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)

# Helper variables to abstracts some common compiler flags.
set(XC32_COMPILER_NO_WARNINGS "-w" CACHE INTERNAL "")

# these settings are specific to cFE/OSAL and determines which 
# abstraction layers are built when using this toolchain
# Note that "pc-linux" works fine even though this is not technically a "pc"
SET(CFE_SYSTEM_PSPNAME      "generic-freertos")
SET(OSAL_SYSTEM_BSPTYPE     "generic-freertos")
SET(OSAL_SYSTEM_OSTYPE      "freertos")

SET(CMAKE_C_FLAGS_INIT "-mfloat-abi=hard -mfpu=fpv5-d16 -mprocessor=ATSAMV71Q21B" CACHE STRING "C flags required by platform")
SET(CMAKE_STATIC_LINKER_FLAGS_INIT "-mfloat-abi=hard -mfpu=fpv5-d16 -mprocessor=ATSAMV71Q21B" CACHE STRING "Linker flags required by platform")
	