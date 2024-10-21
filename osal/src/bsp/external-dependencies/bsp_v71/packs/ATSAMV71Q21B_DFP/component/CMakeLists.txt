######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_dfp_component INTERFACE)

target_include_directories(sam_dfp_component ${CMAKE_CURRENT_SOURCE_DIR})