/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: PacketFileReader.H,v 5.1 2010-10-16 11:01:20 harashima Exp $
*/

#ifndef PacketFileReader_H
#define PacketFileReader_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <pcap.h>

class PacketFileReader {
	friend void pcap_callback(u_char *, const struct pcap_pkthdr *, const u_char *);

	private:

	public:
		PacketFileReader();
		void Init();
		void StartLoop();
		void Loop();
};
void pcap_callback(u_char *, const struct pcap_pkthdr *, const u_char *);

#endif

