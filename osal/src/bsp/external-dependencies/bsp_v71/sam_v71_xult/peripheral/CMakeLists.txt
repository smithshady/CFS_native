######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_bsp_peripheral STATIC)

target_sources( sam_bsp PRIVATE
	*.c
)

add_subdirectory(clk)
add_subdirectory(efc)
add_subdirectory(mcan)
add_subdirectory(mpu)
add_subdirectory(nvic)
add_subdirectory(pio)
add_subdirectory(pwm)
add_subdirectory(rstc)
add_subdirectory(rswdt)
add_subdirectory(spi)
add_subdirectory(tc)
add_subdirectory(twihs)
add_subdirectory(uart)
add_subdirectory(usart)
add_subdirectory(wdt)