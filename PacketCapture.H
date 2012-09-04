/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: PacketCapture.H,v 5.9 2011-11-08 10:20:38 sin Exp $
*/

#ifndef PacketCapture_H
#define PacketCapture_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <pcap.h>


/*
typedef struct pcap_packet_t{
	u_int caplen;
	u_int len;
	timeval ts;
	unsigned char packet[1600];
} pcap_packet;
*/


void pcap_callback(u_char *, const struct pcap_pkthdr *, const u_char *);
void packetcap();

#endif
