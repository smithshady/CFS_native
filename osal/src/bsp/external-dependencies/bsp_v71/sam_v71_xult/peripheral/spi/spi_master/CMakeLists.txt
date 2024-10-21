######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_spi_master STATIC 
	plib_spi0_master.c	
	plib_spi1_master.c	
)

target_include_directories(sam_peripheral_spi ${CMAKE_CURRENT_SOURCE_DIR})
