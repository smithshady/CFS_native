#
# Example arch_build_custom.cmake
# -------------------------------
#
# This file will be automatically included in the arch-specific build scope
#
# Definitions and options specified here will be used when cross-compiling
# _all_ FSW code for _all_ targets defined in targets.cmake.
#
# Avoid machine-specific code generation options in this file (e.g. -f,-m options); such 
# options should be localized to the toolchain file such that they will only be
# included on the machines where they apply.
#
# CAUTION: In heterogeneous environments where different cross compilers are
# used for different CPUs, particularly if from different vendors, it is likely
# that compile options will need to be different as well.
#
# In general, options in this file can only be used in cases where all CPUs use a
# compiler from the same vendor and/or are all GCC based such that they accept similar
# command line options.
#
# This file can alternatively be named as "arch_build_custom_${TARGETSYSTEM}.cmake"
# where ${TARGETSYSTEM} represents the system type, matching the toolchain.
#
# These example options assume a GCC-style toolchain is used for cross compilation,
# and uses the same warning options that are applied at the mission level.
#
add_compile_options(
	-g			#produce debugging information
	-ffunction-sections	# place each function into its own section in the output file
	-fdata-sections 	# place each data item into its own section in the output file
	-O1 			# optimization level 1
	-Wall
)
# TODO evaluate which options should be dropped
