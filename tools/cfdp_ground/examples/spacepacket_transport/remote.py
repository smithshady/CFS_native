import logging

import spacepacket

import cfdp
from cfdp.transport.spacepacket import SpacePacketTransport
from cfdp.transport.udp import UdpTransport
from cfdp.filestore import NativeFileStore


logging.basicConfig(level=logging.DEBUG)

udp_transport = UdpTransport(routing={"*": [("127.0.0.1", 5111)]})
udp_transport.bind("127.0.0.1", 5222)

spacepacket_transport = SpacePacketTransport(
    apid=222, transport=udp_transport, packet_type=spacepacket.PacketType.TELEMETRY
)

cfdp_entity = cfdp.CfdpEntity(
    entity_id=2,
    filestore=NativeFileStore("../files/remote"),
    transport=spacepacket_transport,
)

input("Running. Press <Enter> to stop...\n")

cfdp_entity.shutdown()
udp_transport.unbind()
