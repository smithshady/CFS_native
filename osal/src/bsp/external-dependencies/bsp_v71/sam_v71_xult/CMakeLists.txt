######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_bsp exceptions.c freertos_hooks.c initialization.c interrupts.c libc_syscalls.c startup_xc32.c STATIC)

add_subdirectory(bsp)
add_subdirectory(driver)
add_subdirectory(osal)
add_subdirectory(peripheral)
add_subdirectory(stdio)
add_subdirectory(system)

target_include_directories(sam_bsp ${CMAKE_CURRENT_SOURCE_DIR})