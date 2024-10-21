/**
 * @file default_atsam_test_topicids.h
 * @author Vincent Marias <vincent.marias@lunaroupost.com>
 * @desc CFE Software Bus Topic ID definitions for CMD/TLM interface(s) of the
 * ATSAM Test app.
 */

#ifndef ATSAM_TEST_DEFAULT_ATSAM_TEST_TOPICIDS_H
#define ATSAM_TEST_DEFAULT_ATSAM_TEST_TOPICIDS_H

/* Command Topics */
#define CFE_MISSION_ATSAM_TEST_CMD_TOPICID      0x87
#define CFE_MISSION_ATSAM_TEST_SEND_HK_TOPICID  0x94

/* Telemetry Topics */
#define CFE_MISSION_ATSAM_TEST_HK_TLM_TOPICID   CFE_MISSION_ATSAM_TEST_SEND_HK_TOPICID

/* Network Topics */
#define CFE_MISSION_ATSAM_TEST_SBN_TEST         0x93

#endif /* ATSAM_TEST_DEFAULT_ATSAM_TEST_TOPICIDS_H */
