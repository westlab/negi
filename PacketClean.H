/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: PacketClean.H,v 5.0 2010-10-13 08:47:16 sin Exp $
*/
#pragma once
#include "Include.H"
#include "Packet.H"

class PacketClean
{
	private:
		struct timeval last_gc_time;

	public:
		PacketClean();
		~PacketClean();
		void Proc(Packet *pkt);
};
