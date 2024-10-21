######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_usart STATIC 
	plib_usart0.c	
	plib_usart1.c	
	plib_usart2.c	
)

target_include_directories(sam_peripheral_usart ${CMAKE_CURRENT_SOURCE_DIR})