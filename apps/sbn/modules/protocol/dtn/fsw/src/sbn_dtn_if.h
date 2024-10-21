#ifndef _sbn_dtn_if_h_
#define _sbn_dtn_if_h_

#include "sbn_interfaces.h"
#include "cfe.h"

/* The version #'s this module works with. If this doesn't match the core
 * SBN application version #'s, this module should report and return an error.
 * Note: these DEFINITELY should be hardcoded here, do not map them to
 * the SBN version numbers.
 */
#define SBN_DTN_MAJOR    1
#define SBN_DTN_MINOR    11
#define SBN_DTN_REVISION 0

SBN_Status_t SBN_DTN_Init(int Version, CFE_EVS_EventID_t BaseEID, SBN_ProtocolOutlet_t *Outlet);

SBN_Status_t SBN_DTN_LoadNet(SBN_NetInterface_t *Net, const char *Address);

SBN_Status_t SBN_DTN_LoadPeer(SBN_PeerInterface_t *Peer, const char *Address);

SBN_Status_t SBN_DTN_InitNet(SBN_NetInterface_t *NetInterface);

SBN_Status_t SBN_DTN_InitPeer(SBN_PeerInterface_t *PeerInterface);

SBN_Status_t SBN_DTN_PollPeer(SBN_PeerInterface_t *PeerInterface);

SBN_Status_t SBN_DTN_Send(SBN_PeerInterface_t *Peer, SBN_MsgType_t MsgType, SBN_MsgSz_t MsgSz, void *Payload);

SBN_Status_t SBN_DTN_Recv(SBN_NetInterface_t *Net, SBN_MsgType_t *MsgTypePtr, SBN_MsgSz_t *MsgSzPtr, CFE_ProcessorID_t *CpuIDPtr,
                 CFE_SpacecraftID_t *SpacecraftIDPtr, void *Payload);

SBN_Status_t SBN_DTN_ReportModuleStatus(SBN_ModuleStatusPacket_t *Packet);

SBN_Status_t SBN_DTN_ResetPeer(SBN_PeerInterface_t *PeerInterface);

SBN_Status_t SBN_DTN_UnloadNet(SBN_NetInterface_t *NetInterface);

SBN_Status_t SBN_DTN_UnloadPeer(SBN_PeerInterface_t *PeerInterface);

SBN_IfOps_t SBN_DTN_Ops = {
    .InitModule = SBN_DTN_Init,
    .InitNet = SBN_DTN_InitNet,
    .InitPeer = SBN_DTN_InitPeer,
    .LoadNet = SBN_DTN_LoadNet,
    .LoadPeer = SBN_DTN_LoadPeer,
    .PollPeer = SBN_DTN_PollPeer,
    .Send = SBN_DTN_Send,
    .RecvFromPeer = NULL,
    .RecvFromNet = SBN_DTN_Recv,
    .UnloadNet = SBN_DTN_UnloadNet,
    .UnloadPeer = SBN_DTN_UnloadPeer
                              };

//TODO like other file, check on if reset and reportmodulestatus needed

#endif /* _sbn_dtn_if_h_ */
