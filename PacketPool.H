/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: PacketPool.H,v 5.0 2010-10-13 08:47:16 sin Exp $
*/
#pragma once

#include "Packet.H"

class PacketPool
{
	private:
		list<Packet*> packet_pool;

	public:
		PacketPool();
		~PacketPool();
		
		void RefreshPacket();

		list<Packet*>::iterator GetPacketFirstIt();
		list<Packet*>::iterator GetPacketLastIt();
		list<Packet*>::iterator RemovePacketIt(list<Packet*>::iterator it);

		Packet * AddPacket(Packet* pkt);
		Packet* AddPacket( string content_size, string src_ip, string src_port, string dst_ip, string dst_port, string flag, string content);
		void ShowPackets();
};

