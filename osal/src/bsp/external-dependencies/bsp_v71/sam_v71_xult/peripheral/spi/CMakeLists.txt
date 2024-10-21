######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_peripheral_spi STATIC 
	plib_spi0.c	
	plib_spi1.c	
)

target_include_directories(sam_peripheral_spi ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(spi_master)