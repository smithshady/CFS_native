#include "sbn_tbl.h"
#include "cfe_tbl_filedef.h"

SBN_ConfTbl_t SBN_ConfTbl = {.ProtocolModules = {{/* [0] */
                                                  .Name        = "UDP",
                                                  .LibFileName = "/cf/sbn_udp.so",
                                                  .LibSymbol   = "SBN_UDP_Ops",
                                                  .BaseEID     = 0x0100}},
                             .ProtocolCnt     = 1,
                             .FilterModules   = {{/* [0] */
                                                .Name        = "Remap",
                                                .LibFileName = "/cf/sbn_f_remap.so",
                                                .LibSymbol   = "SBN_F_Remap",
                                                .BaseEID     = 0x1000}},
                             .FilterCnt       = 1,
                             .Peers =
                                 {
                                    {/* [0] */
                                      .ProcessorID  = 3,
                                      .SpacecraftID = 0x42,
                                      .NetNum       = 0,
                                      .ProtocolName = "UDP",
                                      .Filters      = {"Remap"},
                                      .Address      = "127.0.0.1:2234",
                                      .TaskFlags    = SBN_TASK_POLL},
                                    {/* [1] */
                                      .ProcessorID  = 1,
                                      .SpacecraftID = 0x41,
                                      .NetNum       = 0,
                                      .ProtocolName = "UDP",
                                      .Filters      = {"Remap"},
                                      .Address      = "172.40.15.171:2235",
                                      .TaskFlags    = SBN_TASK_POLL},
                                 },
                             .PeerCnt = 1}; // SBN disabled

CFE_TBL_FILEDEF(SBN_ConfTbl, SBN.SBN_ConfTbl, SBN Configuration Table, sbn_conf_tbl.tbl)

/*
For second device (peer)

.Peers =
{
    {
        .ProcessorID  = 1,
        .SpacecraftID = 0x41,
        .NetNum       = 0,
        .ProtocolName = "UDP",
        .Address      = "127.0.0.1:2235",
        .TaskFlags    = SBN_TASK_POLL},
    {
        .ProcessorID  = 3,
        .SpacecraftID = 0x42,
        .NetNum       = 0,
        .ProtocolName = "UDP",
        .Address      = "172.40.15.79:2234",
        .TaskFlags    = SBN_TASK_POLL}
},

*/
