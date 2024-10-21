######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_dfp INTERFACE)

add_subdirectory(component)
add_subdirectory(instance)

target_include_directories(sam_dfp ${CMAKE_CURRENT_SOURCE_DIR} pio)