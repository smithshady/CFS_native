######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_uart STATIC 
	plib_uart0.c	
	plib_uart1.c	
	plib_uart2.c	
	plib_uart3.c	
)

target_include_directories(sam_peripheral_uart ${CMAKE_CURRENT_SOURCE_DIR})