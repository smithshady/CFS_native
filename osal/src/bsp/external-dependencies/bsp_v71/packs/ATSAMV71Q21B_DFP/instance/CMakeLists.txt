######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_dfp_instance INTERFACE)

target_include_directories(sam_dfp_instance ${CMAKE_CURRENT_SOURCE_DIR})