######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_twihs STATIC 
	plib_twihs0.c	
)

target_include_directories(sam_peripheral_twihs ${CMAKE_CURRENT_SOURCE_DIR})