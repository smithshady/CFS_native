######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_pio STATIC 
	plib_pio.c	
)

target_include_directories(sam_peripheral_pio ${CMAKE_CURRENT_SOURCE_DIR})