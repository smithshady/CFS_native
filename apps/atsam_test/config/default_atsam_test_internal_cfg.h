/**
 * @file default_atsam_test_interanl_cfg.h
 * @author Vincent Marias <vincent.marias@lunaroutpost.com>
 * @desc Software configuration that does not affect interfaces, and may be
 * different per instance/platform.
 */

#ifndef ATSAM_TEST_DEFAULT_ATSAM_TEST_INTERNAL_CFG_H
#define ATSAM_TEST_DEFAULT_ATSAM_TEST_INTERNAL_CFG_H

#define ATSAM_TEST_PIPE_NAME    "ATSAM_TEST_CMD_PIPE"
#define ATSAM_TEST_PIPE_DEPTH   10                          /* depth of command pipe */

#define ATSAM_TEST_STR_LEN      32                          /* length of static string in command payload */

#endif /* ATSAM_TEST_DEFAULT_ATSAM_TEST_INTERNAL_CFG_H */
