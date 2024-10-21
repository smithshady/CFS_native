######################################################################
#
# CMAKE build recipe for Generic VxWorks Board Support Package (BSP)
#
######################################################################

add_library(osal_generic-freertos_impl OBJECT
	src/bsp_start.c
	src/bsp_console.c
)

target_link_libraries(osal_generic-freertos_impl PRIVATE sam_packs sam_bsp)

# handle adding filesystem
#list(APPEND OS_BSP_SRCLIST
#	src/little_fs
#) # TODO update OS_BSP_SRCLIST to correct stuff

# handle adding filesystem
#list(APPEND OS_BSP_SRCLIST
#	src/freertos_tcp
#) # TODO update OS_BSP_SRCLIST to correct stuff

target_link_libraries(osal_public_api INTERFACE_AUTOUIC_OPTIONS
	freertoscpu
)

# add bplib here

# The "-u" switch is required to ensure that the linker pulls in the OS_BSPMain entry point
target_link_libraries(osal_public_api INTERFACE -uOS_BSPMain)

# This BSP only works with "freertos" OS layer.
# Confirming this reduces risk of accidental misconfiguration
set_property(TARGET osal_generic-freertos_impl PROPERTY OSAL_EXPECTED_OSTYPE "freertos")
