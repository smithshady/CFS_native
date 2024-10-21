#!/usr/bin/env python3

#
#  NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
#
#  Copyright (c) 2020 United States Government as represented by the
#  Administrator of the National Aeronautics and Space Administration.
#  All Rights Reserved.
#
#  Licensed under the Apache License, Version 2.0 (the "License"); you may
#  not use this file except in compliance with the License. You may obtain
#  a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

import socket
from struct import unpack
from time import sleep, time
import os

import zmq
from PyQt5.QtCore import QThread, pyqtSignal

# Receive port where the CFS TO_Lab app sends the telemetry packets
udp_recv_port = 1235

# Socket to forward cfdp PDU packets to
udp_forward_ip = "127.0.0.1"
udp_forward_port = 5222

#
# Receive telemetry packets, apply the appropriate header
# and publish the message with zeroMQ
#
class RoutingService(QThread):
    # Signal to update the spacecraft combo box (list) on main window GUI
    signal_update_ip_list = pyqtSignal(str, bytes)

    def __init__(self):
        super().__init__()

        # Init lists
        self.ip_addresses_list = ["All"]
        self.spacecraft_names = ["All"]
        self.special_pkt_id = []
        self.special_pkt_name = []

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        # Init zeroMQ
        self.context = zmq.Context()
        self.publisher = self.context.socket(zmq.PUB)
        self.publisher.bind("ipc:///tmp/GroundSystem")

        # Timing variables for measuring file transfer speed
        self.file_forwarding = False
        self.file_transfer_start_time = 0
        self.current_filename = None
        self.pdu_count = 0

    # Run thread
    def run(self):
        # Init udp socket
        self.sock.bind(('', udp_recv_port))

        print('Attempting to wait for UDP messages')

        socket_error_count = 0
        while socket_error_count < 5:

            # Wait for UDP messages
            while True:
                try:
                    # Receive message
                    datagram, host = self.sock.recvfrom(
                        4096)  # buffer size is 1024 bytes

                    # Ignore datagram if it is not long enough (doesn't contain tlm header?)
                    if len(datagram) < 6:
                        continue

                    # Read host address
                    host_ip_address = host[0]

                    #
                    # Add Host to the list if not already in list
                    #
                    if host_ip_address not in self.ip_addresses_list:
                        ## MAKE SURE THERE'S NO SPACE BETWEEN "Spacecraft"
                        ## AND THE FIRST CURLY BRACE!!!
                        hostname = f'Spacecraft{len(self.spacecraft_names)}'
                        my_hostname_as_bytes = hostname.encode()
                        print("Detected", hostname, "at", host_ip_address)
                        self.ip_addresses_list.append(host_ip_address)
                        self.spacecraft_names.append(my_hostname_as_bytes)
                        self.signal_update_ip_list.emit(host_ip_address,
                                                        my_hostname_as_bytes)
                        
                    # If PDU received, forward to cfdp host
                    if datagram[1] == 0xc2:
                        # check for first file PDU
                        if self.pdu_count == 0:
                            # this works to get filename because no characters have an ascii value less than 32. Assumes filename is less than 32 chars
                            filename_length = next((value for value in reversed(datagram) if value < 32), None) - 1
                            self.current_filename = datagram[-1*filename_length:].decode() # must have a "/" at start of filename in the tx file cmd
                            print(f"Got filename: {self.current_filename}")
                            # start timer, timer will end once EOF message received
                            self.file_transfer_start_time = time()
                        self.pdu_count += 1
                        self.sock.sendto(datagram[20:], (udp_forward_ip, udp_forward_port)) # remove cFS tlm header before sending
                        print(f"Forwarded PDU packet {self.pdu_count} to {udp_forward_ip}:{udp_forward_port}")

                    # Once EOF message received, end timing and save data to file
                    if datagram[1] == 0xB3: 
                        print("Received EOF message from cFS")
                        file_transfer_total_time = time() - self.file_transfer_start_time
                        filepath = "../cfdp_ground/files/"
                        file_creation_time = 0
                        file_creation_time_max = 10
                        start_file_creation_time = time()
                        while not os.path.exists(f"{filepath}{self.current_filename}") and file_creation_time < file_creation_time_max:
                            file_creation_time = time() - start_file_creation_time
                            sleep(0.2)
                        if file_creation_time < file_creation_time_max:
                            file_size_bytes = os.path.getsize(f"{filepath}{self.current_filename}")
                            header_string = f"{'=' * len(self.current_filename)}\n{self.current_filename}:\n{'=' * len(self.current_filename)}\n"
                            time_string = f"downlink time: {file_transfer_total_time:.6f}s\n"
                            size_string = f"filesize: {file_size_bytes}B | {(file_size_bytes / 1024):.2f}KB | {self.pdu_count} PDUs\n"
                            speed_string = f"downlink speed: {(file_size_bytes / file_transfer_total_time):.2f}B/s | {((file_size_bytes / 1024) / file_transfer_total_time):.2f}KB/s\n"
                            creation_string = f"file creation time: {file_creation_time:.2f}s\n"
                            print(header_string, end='')
                            print(time_string, end='')
                            print(size_string, end='')
                            print(speed_string, end='')
                            print(creation_string, end='')
                            with open(f"{filepath}/performance_test/file_performance.txt", 'a') as file:
                                file.write(header_string)
                                file.write(time_string)
                                file.write(size_string)
                                file.write(speed_string)
                        else:
                            print("File not created in time (10s)")
                        self.pdu_count = 0
                        self.current_filename = None

                    # Else forward the message using zeroMQ
                    else:
                        name = self.spacecraft_names[self.ip_addresses_list.index(host_ip_address)]
                        self.forwardMessage(datagram, name)

                # Handle errors
                except socket.error:
                    print('Ignored socket error for attempt', socket_error_count)
                    socket_error_count += 1
                    sleep(1)

    # Apply header using hostname and packet id and send msg using zeroMQ
    def forwardMessage(self, datagram, hostName):
        # Forward message to channel GroundSystem.<hostname>.<pkt_id>
        pkt_id = self.get_pkt_id(datagram)
        my_decoded_hostname = hostName.decode()
        header = f"GroundSystem.{my_decoded_hostname}.TelemetryPackets.{pkt_id}"
        my_header_as_bytes = header.encode()
        self.publisher.send_multipart([my_header_as_bytes, datagram])
        # print(header)

    # Read the packet id from the telemetry packet
    @staticmethod
    def get_pkt_id(datagram):
        # Read the telemetry header
        stream_id = unpack(">H", datagram[:2])
        return hex(stream_id[0])

    # Close ZMQ vars
    def stop(self):
        self.sock.close()
        self.context.destroy()
