######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_mpu STATIC 
	plib_mpu.c	
)

target_include_directories(sam_peripheral_mpu ${CMAKE_CURRENT_SOURCE_DIR})