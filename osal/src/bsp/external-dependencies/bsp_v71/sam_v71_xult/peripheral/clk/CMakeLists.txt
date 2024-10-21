######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_clk STATIC 
	plib_clk.c	
)

target_include_directories(sam_peripheral_clk ${CMAKE_CURRENT_SOURCE_DIR})