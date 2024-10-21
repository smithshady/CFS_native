######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_cmsis INTERFACE)

target_include_directories(sam_cmsis ${CMAKE_CURRENT_SOURCE_DIR} CMSIS/Core/Include)