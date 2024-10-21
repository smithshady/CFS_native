######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_efc STATIC 
	plib_efc.c	
)

target_include_directories(sam_peripheral_efc ${CMAKE_CURRENT_SOURCE_DIR})