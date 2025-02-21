#include "sbn_dtn_if_struct.h"
#include "sbn_dtn_if.h"
#include "sbn_dtn_events.h"
#include "cfe.h"
#include "sbn_app.h"
#include <string.h>
#include <errno.h>
#include <sys/select.h>


CFE_EVS_EventID_t SBN_DTN_FIRST_EID;

static SBN_ProtocolOutlet_t SBN_out;

SBN_Status_t SBN_DTN_Init(int Version, CFE_EVS_EventID_t BaseEID, SBN_ProtocolOutlet_t *Outlet)
{
    SBN_DTN_FIRST_EID = BaseEID;

    if (Version != 1)
    {
        CFE_EVS_SendEvent(SBN_DTN_CONFIG_EID, CFE_EVS_EventType_ERROR, "mismatching version %d (SBN app reports %d)", Version, 1);
        return SBN_ERROR;
    } /* end if */
    
    memcpy(&SBN_out, Outlet, sizeof(SBN_out));

    return SBN_SUCCESS;
} /* end SBN_DTN_Init() */

SBN_Status_t SBN_DTN_LoadNet(SBN_NetInterface_t *Net, const char *Address)
{
    SBN_DTN_Net_t *NetData = (SBN_DTN_Net_t *)Net->ModulePvt;

    strncpy(NetData->EIN, Address, sizeof(NetData->EIN));

    return SBN_SUCCESS;
} /* end SBN_DTN_LoadNet */

SBN_Status_t SBN_DTN_LoadPeer(SBN_PeerInterface_t *Peer, const char *Address)
{
    SBN_DTN_Peer_t *PeerData = (SBN_DTN_Peer_t *)Peer->ModulePvt;

    strncpy(PeerData->EIN, Address, sizeof(PeerData->EIN));

    return SBN_SUCCESS;
} /* end SBN_DTN_LoadHost */

/**
 * Initializes a DTN host.
 *
 * @param  Interface data structure containing the file entry
 * @return SBN_SUCCESS on success, error code otherwise
 */
SBN_Status_t SBN_DTN_InitNet(SBN_NetInterface_t *Net)
{
    SBN_DTN_Net_t *NetData = (SBN_DTN_Net_t *)Net->ModulePvt;

    if (bp_attach() < 0)
    {
        CFE_EVS_SendEvent(SBN_DTN_SOCK_EID, CFE_EVS_EventType_ERROR, "unable to attach to DTN BP");
        return SBN_ERROR;
    } /* end if */

    if (ionStartAttendant(&NetData->Attendant))
    {
        CFE_EVS_SendEvent(SBN_DTN_SOCK_EID, CFE_EVS_EventType_ERROR, "unable to start attendant");
        return SBN_ERROR;
    } /* end if */

    if (bp_open(NetData->EIN, &NetData->SAP) < 0)
    {
        CFE_EVS_SendEvent(SBN_DTN_SOCK_EID, CFE_EVS_EventType_ERROR, "unable to open EIN %s", NetData->EIN);
        return SBN_ERROR;
    } /* end if */

    NetData->SendSDR = bp_get_sdr();
    NetData->RecvSDR = bp_get_sdr();

    return SBN_SUCCESS;
} /* end SBN_DTN_InitNet */

/**
 * Initializes DTN peer data struct depending on the
 * CPU name.
 *
 * @param  Interface data structure containing the file entry
 * @return SBN_SUCCESS on success, error code otherwise
 */
SBN_Status_t SBN_DTN_InitPeer(SBN_PeerInterface_t *Peer)
{
    SBN_Connected(Peer);

    return SBN_SUCCESS;
} /* end SBN_DTN_InitPeer */

/**
 */
SBN_Status_t SBN_DTN_PollPeer(SBN_PeerInterface_t *Peer)
{
    return SBN_SUCCESS;
} /* end SBN_DTN_PollPeer */

SBN_Status_t SBN_DTN_Send(SBN_PeerInterface_t *Peer, SBN_MsgType_t MsgType, SBN_MsgSz_t MsgSz, void *Payload)
{
    uint8 SendBuf[SBN_MAX_PACKED_MSG_SZ];

    SBN_DTN_Peer_t *PeerData = (SBN_DTN_Peer_t *)Peer->ModulePvt;
    SBN_DTN_Net_t * NetData  = (SBN_DTN_Net_t *)Peer->Net->ModulePvt;

    SBN_PackMsg(&SendBuf, MsgSz, MsgType, CFE_PSP_GetProcessorId(), CFE_PSP_GetSpacecraftId(), Payload);

    CHKZERO(sdr_begin_xn(NetData->SendSDR));

    Object Extent = sdr_malloc(NetData->SendSDR, sizeof(SendBuf));
    if (!Extent)
    {
        CFE_EVS_SendEvent(SBN_DTN_SOCK_EID, CFE_EVS_EventType_ERROR, "unable to allocate extent");
        return SBN_ERROR;
    } /* end if */

    sdr_write(NetData->SendSDR, Extent, (char *)&SendBuf, sizeof(SendBuf));

    if (sdr_end_xn(NetData->SendSDR) < 0)
    {
        CFE_EVS_SendEvent(SBN_DTN_SOCK_EID, CFE_EVS_EventType_ERROR, "no space for ZCO extent");
        return SBN_ERROR;
    } /* end if */

    Object BundleZCO =
        ionCreateZco(ZcoSdrSource, Extent, 0, sizeof(SendBuf), BP_STD_PRIORITY, 0, ZcoOutbound, &NetData->Attendant);
    if (BundleZCO == 0 || BundleZCO == (Object)ERROR)
    {
        CFE_EVS_SendEvent(SBN_DTN_SOCK_EID, CFE_EVS_EventType_ERROR, "can't create ZCO extent");
        return SBN_ERROR;
    } /* end if */

    Object NewBundle;
    if (bp_send(NULL, PeerData->EIN, NULL, 300, BP_STD_PRIORITY, NoCustodyRequested, 0, 0, NULL, BundleZCO,
                &NewBundle) < 1)
    {
        CFE_EVS_SendEvent(SBN_DTN_SOCK_EID, CFE_EVS_EventType_ERROR, "bpsource can't send ADU");
        return SBN_ERROR;
    } /* end if */

    return SBN_SUCCESS;
} /* end SBN_DTN_Send */

/* Note that this Recv function is indescriminate, packets will be received
 * from all peers but that's ok, I just inject them into the SB and all is
 * good!
 */
SBN_Status_t SBN_DTN_Recv(SBN_NetInterface_t *Net, SBN_MsgType_t *MsgTypePtr, SBN_MsgSz_t *MsgSzPtr, CFE_ProcessorID_t *CpuIDPtr,
                CFE_SpacecraftID_t *SpacecraftIDPtr, void *Payload)
{
    uint8      RecvBuf[SBN_MAX_PACKED_MSG_SZ];
    BpDelivery Delivery;
    int        TimeoutSecs = 0;

#ifdef SBN_RECV_TASK
    /* task-based peer connections block on reads, otherwise use select */
    TimeoutSecs = BP_BLOCKING;
#endif /* SBN_RECV_TASK */

    SBN_DTN_Net_t *NetData = (SBN_DTN_Net_t *)Net->ModulePvt;

    if (bp_receive(NetData->SAP, &Delivery, TimeoutSecs) < 0)
    {
        CFE_EVS_SendEvent(SBN_DTN_SOCK_EID, CFE_EVS_EventType_ERROR, "BP receive returned an error");
        return SBN_ERROR;
    } /* end if */

    switch (Delivery.result)
    {
        case BpReceptionTimedOut:
            /* fall through */

        case BpReceptionInterrupted:
            /* fall through */

        case BpEndpointStopped:
            /* should return something to indicate the connection lost */
            return SBN_IF_EMPTY;

        case BpPayloadPresent:
            /* got a payload, break and continue processing */
            break;

        default:
            return SBN_SUCCESS; /* ? */
    }

    /* got a payload, process */

    CHKZERO(sdr_begin_xn(NetData->RecvSDR));
    int ContentLength = zco_source_data_length(NetData->RecvSDR, Delivery.adu);
    sdr_exit_xn(NetData->RecvSDR);
    if (ContentLength > sizeof(RecvBuf))
    {
        CFE_EVS_SendEvent(SBN_DTN_SOCK_EID, CFE_EVS_EventType_ERROR, "Received message too large for buffer. (Bufsize=%d Msg=%d)",
                          sizeof(RecvBuf), ContentLength);
        return SBN_ERROR;
    } /* end if */

    ZcoReader Reader;
    zco_start_receiving(Delivery.adu, &Reader);
    CHKZERO(sdr_begin_xn(NetData->RecvSDR));
    int Len = zco_receive_source(NetData->RecvSDR, &Reader, ContentLength, (char *)&RecvBuf);
    if (sdr_end_xn(NetData->RecvSDR) < 0 || Len < 0)
    {
        CFE_EVS_SendEvent(SBN_DTN_SOCK_EID, CFE_EVS_EventType_ERROR, "Can't handle delivery.");
        return SBN_ERROR;
    } /* end if */

    SBN_UnpackMsg(&RecvBuf, MsgSzPtr, MsgTypePtr, CpuIDPtr, Payload);

    return SBN_SUCCESS;
} /* end SBN_DTN_Recv */

SBN_Status_t SBN_DTN_ReportModuleStatus(SBN_ModuleStatusPacket_t *Packet)
{
    return SBN_NOT_IMPLEMENTED;
} /* end SBN_DTN_ReportModuleStatus */

SBN_Status_t SBN_DTN_ResetPeer(SBN_PeerInterface_t *Peer)
{
    return SBN_NOT_IMPLEMENTED;
} /* end SBN_DTN_ResetPeer */

SBN_Status_t SBN_DTN_UnloadNet(SBN_NetInterface_t *Net)
{
    SBN_DTN_Net_t *NetData = (SBN_DTN_Net_t *)Net->ModulePvt;

    ionStopAttendant(&NetData->Attendant);
    bp_detach();

    int PeerIdx = 0;
    for (PeerIdx = 0; PeerIdx < Net->PeerCnt; PeerIdx++)
    {
        SBN_DTN_UnloadPeer(&Net->Peers[PeerIdx]);
    } /* end for */

    return SBN_SUCCESS;
} /* end SBN_DTN_ResetPeer */

SBN_Status_t SBN_DTN_UnloadPeer(SBN_PeerInterface_t *Peer)
{
    return SBN_SUCCESS;
} /* end SBN_DTN_ResetPeer */
