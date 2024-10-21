#ifndef _sbn_dtn_if_struct_h_
#define _sbn_dtn_if_struct_h_

#include "sbn_interfaces.h"
#include "sbn_platform_cfg.h"
#include "cfe.h"
#include <stdbool.h>
//#include <bp.h>

typedef struct
{
    char    EIN[32];
    bool SubsSent;
} SBN_DTN_Peer_t;

typedef struct
{
    char         EIN[32];
    uint32_t        SAP;
    uint32_t          SendSDR, RecvSDR;
    uint32_t Attendant;
} SBN_DTN_Net_t;
/*
typedef struct
{
    char         EIN[32];
    BpSAP        SAP;
    Sdr          SendSDR, RecvSDR;
    ReqAttendant Attendant;
} SBN_DTN_Net_t;
*/
#endif /* _sbn_dtn_if_struct_h_ */
