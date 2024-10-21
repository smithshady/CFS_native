######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_bsp_osal STATIC 
	osal_freertos.c	
)

target_include_directories(sam_bsp_osal ${CMAKE_CURRENT_SOURCE_DIR})