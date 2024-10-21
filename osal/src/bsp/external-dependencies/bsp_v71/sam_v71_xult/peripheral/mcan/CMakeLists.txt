######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_mcan STATIC 
	plib_mcan1.c	
)

target_include_directories(sam_peripheral_mcan ${CMAKE_CURRENT_SOURCE_DIR})