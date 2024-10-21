######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_packs INTERFACE)

add_subdirectory(ATSAMV71Q21B_DFP)
add_subdirectory(CMSIS)