/**
 * @file atsam_test_eventids.h
 * @author Vincent Marias <vincent.marias@lunaroutpost.com>
 * @desc Public header that defines event IDs for the ATSAM Test app. These are
 * used when sending events to EVS (EVent Services).
 */

#ifndef ATSAM_TEST_EVENTIDS_H
#define ATSAM_TEST_EVENTIDS_H

#define ATSAM_TEST_INIT_INF_EID         0   /* INITialization INFormation */

/* errors */
#define ATSAM_TEST_PIPE_ERR_EID         1   /* errors creating/using/tearing down command pipe */
#define ATSAM_TEST_MID_ERR_EID          2   /* received message with invalid ID */
#define ATSAM_TEST_TBL_ERR_EID          3   /* error creating/accessing tables */
#define ATSAM_TEST_CMD_ERR_EID          4   /* received ground command with invalid command code */

/* ground commands */
#define ATSAM_TEST_NOOP_EID             5   /* received noop command */
#define ATSAM_TEST_RESET_COUNTERS_EID   6   /* received "reset counters" command */
#define ATSAM_TEST_DUMP_PAYLOAD_EID     7   /* received "dump payload" command */

#endif /* ATSAM_TEST_EVENTIDS_H */
