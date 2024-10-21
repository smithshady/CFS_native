###########################################################
#
# ATSAM_TEST mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the ATSAM_TEST configuration
set(ATSAM_TEST_MISSION_CONFIG_FILE_LIST
    atsam_test_fcncodes.h
    #atsam_test_interface_cfg.h
    #atsam_test_mission_cfg.h
    #atsam_test_perfids.h
    #atsam_test_msg.h
    atsam_test_msgdefs.h
    atsam_test_msgstruct.h
    #atsam_test_tbl.h
    atsam_test_tbldefs.h
    #atsam_test_tblstruct.h
    atsam_test_topicids.h
)

#if (CFE_EDS_ENABLED_BUILD)
#
#  # In an EDS-based build, these files come generated from the EDS tool
#  set(ATSAM_TEST_CFGFILE_SRC_sample_app_interface_cfg "sample_app_eds_designparameters.h")
#  set(ATSAM_TEST_CFGFILE_SRC_sample_app_tbldefs       "sample_app_eds_typedefs.h")
#  set(ATSAM_TEST_CFGFILE_SRC_sample_app_tblstruct     "sample_app_eds_typedefs.h")
#  set(ATSAM_TEST_CFGFILE_SRC_sample_app_msgdefs       "sample_app_eds_typedefs.h")
#  set(ATSAM_TEST_CFGFILE_SRC_sample_app_msgstruct     "sample_app_eds_typedefs.h")
#  set(ATSAM_TEST_CFGFILE_SRC_sample_app_fcncodes      "sample_app_eds_cc.h")
#
#endif(CFE_EDS_ENABLED_BUILD)

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(ATSAM_TEST_CFGFILE ${ATSAM_TEST_MISSION_CONFIG_FILE_LIST})
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
