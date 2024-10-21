###########################################################
#
# ATSAM_TEST platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the ATSAM_TEST configuration
set(ATSAM_TEST_PLATFORM_CONFIG_FILE_LIST
    atsam_test_internal_cfg.h
    #atsam_test_platform_cfg.h
    #atsam_test_perfids.h
    atsam_test_msgids.h
)

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(ATSAM_TEST_CFGFILE ${ATSAM_TEST_PLATFORM_CONFIG_FILE_LIST})
  get_filename_component(CFGKEY "${ATSAM_TEST_CFGFILE}" NAME_WE)
  if (DEFINED ATSAM_TEST_CFGFILE_SRC_${CFGKEY})
    set(DEFAULT_SOURCE GENERATED_FILE "${ATSAM_TEST_CFGFILE_SRC_${CFGKEY}}")
  else()
    set(DEFAULT_SOURCE FALLBACK_FILE "${CMAKE_CURRENT_LIST_DIR}/config/default_${ATSAM_TEST_CFGFILE}")
  endif()
  generate_config_includefile(
    FILE_NAME           "${ATSAM_TEST_CFGFILE}"
    ${DEFAULT_SOURCE}
  )
endforeach()
