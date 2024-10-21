######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

add_library(sam_system STATIC 
	cache/sys_cache.c int/src/sys_int.c time/src/sys_time.c
)

target_include_directories(sam_system ${CMAKE_CURRENT_SOURCE_DIR} cache debug int time)