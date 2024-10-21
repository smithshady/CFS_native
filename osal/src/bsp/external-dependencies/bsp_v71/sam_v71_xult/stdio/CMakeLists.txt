######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_stdio STATIC 
	xc32_monitor.c	
)