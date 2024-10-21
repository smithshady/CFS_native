######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_nvic STATIC 
	plib_nvic.c	
)

target_include_directories(sam_peripheral_nvic ${CMAKE_CURRENT_SOURCE_DIR})