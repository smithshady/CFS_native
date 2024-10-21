######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_wdt STATIC 
	plib_wdt.c	
)

target_include_directories(sam_peripheral_wdt ${CMAKE_CURRENT_SOURCE_DIR})