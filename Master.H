/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Master.H,v 5.6 2012-03-25 13:21:19 sin Exp $
*/
#pragma once
#include "Include.H"
#include "Master.H"
#include "Packet.H"
#include <ext/stdio_filebuf.h>



#ifdef MASTER_DEBUG_EN
#define MASTER_DEBUG(x)  x ;
#else
#define MASTER_DEBUG(x)   ;
#endif


class Master
{
	private:
		int observer_packet_counter;
		long long all_packet_counter;
		time_t start_time;

	public:
		Master();
		~Master();
		void Loop();
		void Proc(Packet *pkt);
};
