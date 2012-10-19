/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/* 
 * $Id: RiverGate.H,v 5.2 2012-03-21 14:56:25 sin Exp $
*/

#ifndef RiverGate_H
#define RiverGate_H

#include "Include.H"
//#include "Stream.H"
//#include "Packet.H"

//#define SZTXT 512
//#define SZINT 4
#define RIVERPORT 12322
#define RIVERHOST "localhost"

class RiverGate{
	private:
	int sockfd;
	string str_temp;

	public:
		RiverGate();
		~RiverGate();
		int connectCli(const int port, const char hostname[]);
		void ProcStream(Stream * stream);
		void ProcPacket(Packet * pkt);
		void ProcResult(u_int stream_id, u_int rule_id, u_char *result_string, u_int result_size);
};

#endif
