import logging
import cfdp
from cfdp.transport.udp import UdpTransport
from cfdp.filestore import NativeFileStore

logging.basicConfig(level=logging.DEBUG)

# Create UDP transport
udp_transport = UdpTransport(routing={"*": [("127.0.0.1", 5111)]})
udp_transport.bind("127.0.0.1", 5222)

# Initialize CFDP entity
cfdp_entity = cfdp.CfdpEntity(
    entity_id=2, filestore=NativeFileStore("files"), transport=udp_transport
)

# Run until quit
input("Running. Press <Enter> to stop...\n")
cfdp_entity.shutdown()
udp_transport.unbind()
