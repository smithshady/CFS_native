######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_rswdt STATIC 
	plib_rswdt.c	
)

target_include_directories(sam_peripheral_rswdt ${CMAKE_CURRENT_SOURCE_DIR})