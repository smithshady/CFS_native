#include "sbn_remap_tbl.h"
#include "cfe_tbl_filedef.h"
#include "atsam_test_msgids.h"

SBN_RemapTbl_t SBN_RemapTbl = {
    .RemapDefaultFlag = SBN_REMAP_DEFAULT_IGNORE,
    .Entries          = {{.ProcessorID = 1, .SpacecraftID = 0x41, .FromMID = {ATSAM_TEST_SBN_TEST}, .ToMID = {ATSAM_TEST_SBN_TEST}},

                /** ProcessorID "0" signals the end of the table. */
                {.ProcessorID = 0, .SpacecraftID = 0, .FromMID = {0x0000}, .ToMID = {0x0000}}}}; /* end SBN_RemapTbl */

CFE_TBL_FILEDEF(SBN_RemapTbl, SBN.SBN_RemapTbl, SBN Remap Table, sbn_remap_tbl.tbl)
