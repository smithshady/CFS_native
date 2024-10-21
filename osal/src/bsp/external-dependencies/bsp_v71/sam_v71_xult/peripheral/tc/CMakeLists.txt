######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_tc STATIC 
	plib_tc3.c	
)

target_include_directories(sam_peripheral_tc ${CMAKE_CURRENT_SOURCE_DIR})
