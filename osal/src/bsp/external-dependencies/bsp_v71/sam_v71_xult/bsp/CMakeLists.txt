######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_bsp_init STATIC 
	bsp.c
)

target_include_directories(sam_bsp_init ${CMAKE_CURRENT_SOURCE_DIR})