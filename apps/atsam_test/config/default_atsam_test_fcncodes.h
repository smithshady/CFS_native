/**
 * @file default_atsam_test_fcncodes.h
 * @author Vincent Marias <vincent.marias@lunaroutpost.com>
 * @desc Defines command codes for the ATSAM Test app.
 */

#ifndef ATSAM_TEST_DEFAULT_ATSAM_TEST_FCNCODES_H
#define ATSAM_TEST_DEFAULT_ATSAM_TEST_FCNCODES_H

/* basic commands required for every app */
#define ATSAM_TEST_NOOP_CC              0
#define ATSAM_TEST_RESET_COUNTERS_CC    1

/* custom commands */
#define ATSAM_TEST_DUMP_PAYLOAD_CC      2
#define ATSAM_TEST_SEND_NETWORK         3

#endif /* ATSAM_TEST_DEFAULT_ATSAM_TEST_FCNCODES_H */
