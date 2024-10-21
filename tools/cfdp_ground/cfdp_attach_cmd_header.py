import socket

class MiniCmdUtil:

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def __init__(self,
                 host="127.0.0.1",
                 port=1236,
                 endian="LE",
                 pkt_id="18c8", # 0x1801 is event services, 0x18c8 is CF input
                 cmd_code=0,
                 payload=bytes()):

        self.host = host
        self.port = int(port)
        self.endian = "big" if endian == "BE" else "little"
        self.pkt_id = int(pkt_id, 16)
        self.cmd_code = int(cmd_code)
        self.payload = payload
        self.packet = bytearray()
        self.cmd_offset_pri = 4
        self.cmd_offset_sec = 0
        self.checksum = 0xFF
        self.cfs_cmd_sec_hdr = bytearray(2)

    def assemble_pri_header(self):
        ccsds_pri = bytearray(6)
        ccsds_pri[:2] = self.pkt_id.to_bytes(2, byteorder='big')
        ccsds_pri[2:4] = (0xC000).to_bytes(2, byteorder='big')
        total_packet_len = len(ccsds_pri) + len(self.cfs_cmd_sec_hdr)
        total_packet_len += len(self.payload)
        total_packet_len += self.cmd_offset_pri + self.cmd_offset_sec
        ccsds_pri[4:] = (total_packet_len - 7).to_bytes(2, byteorder="big")
        return ccsds_pri

    def assemble_packet(self):
        pri_header = self.assemble_pri_header()
        self.packet.extend(pri_header)
        pri_offset = bytearray(self.cmd_offset_pri)
        self.packet.extend(pri_offset)
        self.cfs_cmd_sec_hdr[0] = self.cmd_code
        sec_offset = bytearray(self.cmd_offset_sec)
        for b in b''.join((pri_header, pri_offset, self.cfs_cmd_sec_hdr, sec_offset,
                           self.payload)):
            self.checksum ^= b
        self.cfs_cmd_sec_hdr[1] = self.checksum
        self.packet.extend(self.cfs_cmd_sec_hdr)
        self.packet.extend(sec_offset)
        self.packet.extend(self.payload)
        self.checksum = 0xFF

    def send_packet(self):
        self.assemble_packet()
        print("Data to send:")
        for i, v in enumerate(self.packet):
            print(f"0x{format(v, '02X')}", end=" ")
            if (i + 1) % 8 == 0:
                print()
        print()
        bytes_sent = self.sock.sendto(self.packet, (self.host, self.port))
        return bytes_sent > 0
    

### MAIN ###

# Setup socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', 5333)) # bind to receive port
print("Waiting for PDUs to attach headers...")

# Receive loop
while True:
    pdu_data, host = sock.recvfrom(4096)
    if len(pdu_data) > 768:
        pdu_data = pdu_data[:768]       # truncate message to the maximum CI ingest size of 768

    # Attach the CCSDS header to the pdu packet
    mcu = MiniCmdUtil(payload=pdu_data)

    # Send packet
    mcu.send_packet()