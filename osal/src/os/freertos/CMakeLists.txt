######################################################################
#
# CMAKE build recipe for FreeRTOS OSAL implementation
#
######################################################################

# This CMake script generates targets specific to the FreeRTOS implementation


# The basic set of files which are always built
set(FREERTOS_BASE_SRCLIST
    src/os-impl-binsem.c
    src/os-impl-common.c
    src/os-impl-console.c
    src/os-impl-countsem.c
    #src/os-impl-errors.c
    src/os-impl-idmap.c
    src/os-impl-mutex.c
    src/os-impl-queues.c
    src/os-impl-tasks.c
    src/os-impl-timebase.c
)

#if(NOT DEFINED CUSTOM_FREERTOS_FILESYSTEM)
#    message(FATAL_ERROR "Must set CUSTOM_FREERTOS_FILESYSTEM for bsp in arch_build_custom.cmake, provide OS_FreeRTOS_FileSysAPI_Impl_Init implementation");
#endif()

set(FREERTOS_TODO_SRCLIST
    src/os-impl-files.c
    src/os-impl-filesys.c
    src/os-impl-dirs.c
    src/os-impl-module.c
    src/os-impl-network.c
    src/os-impl-heap.c
    src/os-impl-sockets
    src/os-impl-time.c
)

# Use portable blocks for basic I/O
# TODO check on this block
set(FREERTOS_IMPL_SRCLIST
    ../portable/os-impl-posix-gettime.c
    ../portable/os-impl-console-bsp.c
    ../portable/os-impl-posix-io.c
    ../portable/os-impl-posix-files.c
    ../portable/os-impl-posix-dirs.c
    ../portable/os-impl-no-condvar.c
)

# Currently the "shell output to file" for FREERTOS is not implemented
list(APPEND FREERTOS_IMPL_SRCLIST
    ../portable/os-impl-console-bsp.c
)

# If some form of module loading is configured,
# then build the module loader
# TODO disable module loading (see vxworks impl)
if (OSAL_CONFIG_INCLUDE_DYNAMIC_LOADER)
    list(APPEND FREERTOS_IMPL_SRCLIST
        ../portable/os-impl-no-loader.c                    # Use RTEMS-specific versions of the load/unload routines
        #../portable/os-impl-posix-dl-symtab.c   # Use Standard POSIX implementation for symbol lookups
    )
else ()
    list(APPEND FREERTOS_IMPL_SRCLIST
        src/os-impl-no-module.c                 # No-op init routine
        ../portable/os-impl-no-loader.c         # Non-implemented stubs for all Impl routines
        ../portable/os-impl-no-symtab.c
    )
endif ()


# Leverage the "standard" BSD socket code if network is enabled
# TODO disable network and sockets (see vxworks impl)
#if (OSAL_CONFIG_INCLUDE_NETWORK)
#    list(APPEND FREERTOS_IMPL_SRCLIST
#        src/os-impl-network.c
#        ../portable/os-impl-bsd-sockets.c
#        ../portable/os-impl-bsd-select.c
#    )
#    target_link_libraries(osal_public_api INTERFACE
#        FreeRTOS-Plus-TCP 
#    )
#else()
    list(APPEND FREERTOS_IMPL_SRCLIST
        ../portable/os-impl-no-network.c
        ../portable/os-impl-no-sockets.c
        ../portable/os-impl-no-select.c
    )
#endif ()

# Defines an OBJECT target named "osal_freertos_impl" with selected source files
add_library(osal_freertos_impl OBJECT
    ${FREERTOS_BASE_SRCLIST}
    ${FREERTOS_IMPL_SRCLIST}
)

target_include_directories(osal_freertos_impl PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_compile_definitions(osal_public_api INTERFACE
    _FREERTOS_OS_
)
#todo this probably needs to be at a higher level
target_link_libraries(osal_freertos_impl PRIVATE freertos_kernel)
