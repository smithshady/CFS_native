######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_pwm STATIC 
	plib_pwm0.c	
	plib_pwm1.c	
)

target_include_directories(sam_peripheral_pwm ${CMAKE_CURRENT_SOURCE_DIR})