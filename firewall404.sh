#!/bin/sh

############################################
#	Homework Number:	09
#	Name:				Gavin Neyman
#	ECN login:			gneyman
#	Due Date:			3/28/2019
############################################

# Remove any previous rules or chains
iptables -t filter -F
iptables -t filter -X
iptables -t mangle -F
iptables -t mangle -X
iptables -t nat -F
iptables -t nat -X
iptables -t raw -F
iptables -t raw -X

# For all outgoing packets, change their source IP address to your own machine's IP address
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE

# Block a list of specific IPs for all incoming communications
randomIPs=(100.100.100.100 10.10.10.10 50.50.50.50)
for specificIP in $randomIPs
do
	iptables -A INPUT -s $specificIP -j REJECT
	iptables -A OUTPUT -d $specificIP -j REJECT
done

# Block the computer from being pinged by all other hosts
iptables -A INPUT -p icmp --icmp-type echo-request -j DROP

# Set up port-forwarding from a specified port to port 22 and enable connections to specified port
port_of_my_choice=111
iptables -A INPUT -p tcp --destination-port $port_of_my_choice -j ACCEPT
iptables -t nat -A PREROUTING -p tcp -dport $port_of_my_choice -j REDIRECT --to-port 22

# Allow for SSH into the machine from only engineering.purdue.edu domain
iptables -A INPUT -p tcp --src 128.46.105.5 --destination-port 22 -j ACCEPT

# Allow a IP address in the internet to access your machine for the HTTP service
iptables -A INPUT -p tcp --destination-port 80 -j ACCEPT

# Permit Auth/Ident
iptables -p tcp --dport 113 -j ACCEPT
iptables -p udp --dport 113 -j ACCEPT
