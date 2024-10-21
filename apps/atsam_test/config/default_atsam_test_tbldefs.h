/**
 * @file default_atsam_test_tbledefs.h
 * @author Vincent Marias <vincent.marias@lunaroupost.com>
 * @desc Provides type definitions for the application's tables.
 */

#ifndef ATSAM_TEST_DEFAULT_ATSAM_TEST_TBLDEFS_H
#define ATSAM_TEST_DEFAULT_ATSAM_TEST_TBLDEFS_H

#include "common_types.h"

/* Table 1 */
struct ATSAM_TEST_Table1_t {
    uint16 Val1, Val2;
};

/* define filename for table imgae */
#define ATSAM_TEST_TABLE1_FILE "/cf/atsam_test_tbl1.tbl"

#endif /* ATSAM_TEST_DEFAULT_ATSAM_TEST_TBLDEFS_H */
