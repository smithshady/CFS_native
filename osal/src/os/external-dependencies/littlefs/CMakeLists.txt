######################################################################
#
# CMAKE build recipe for freeRTOS kernel 
#
######################################################################

cmake_minimum_required(VERSION 3.15)

project(littlefs STATIC)

# TODO verify lfs target
add_custom_target(BuildLittleFs ALL 
	COMMAND make
)
add_library(LittleFs STATIC IMPORTED)
set_target_properties(LittleFs
	IMPORTED_LOCATION ${LFS_DIR}/liblfs.a
	INTERFACE_INCLUDE_DIRECTORIES ${LFS_DIR}
	INTERFACE_COMPILE_DEFINITIONS "USING_LITTLEFS;LITTLEFS_STATIC"
)
add_dependencies(LittleFs BuildLittleFs)