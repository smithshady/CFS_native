project(CFE_ATSAM_TEST C)

set(APP_SRC_FILES
    fsw/src/atsam_test_app.c
    fsw/src/atsam_test_dispatch.c
    fsw/src/atsam_test_cmds.c
)

# Create the app module
add_cfe_app(atsam_test ${APP_SRC_FILES})

target_include_directories(atsam_test PUBLIC fsw/inc)

# Add table
add_cfe_tables(atsam_test fsw/tables/atsam_test_tbl1.c)
