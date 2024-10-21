######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_bsp_driver INTERFACE)

target_include_directories(sam_bsp_driver ${CMAKE_CURRENT_SOURCE_DIR})