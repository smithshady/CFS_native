/**
 * @file atsam_test_tbl1.c
 * @author Vincent Marias <vincent.marias@lunaroutpost.com>
 * @desc
 */

#include "cfe_tbl_filedef.h"
#include "atsam_test_tbldefs.h"

/* define the contents of the table image */
struct ATSAM_TEST_Table1_t Tbl1 = { 1024, 2048 };

/*
** The macro below identifies:
**    1) the data structure type to use as the table image format
**    2) the name of the table to be placed into the cFE Example Table File Header
**    3) a brief description of the contents of the file image
**    4) the desired name of the table image binary file that is cFE compatible
*/
CFE_TBL_FILEDEF(Tbl1, ATSAM_TEST.Table1, Table Utility Test Table, atsam_test_tbl1.tbl)
