/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: PacketPool.C,v 5.0 2010-10-13 08:47:16 sin Exp $
*/

#include "PacketPool.H"
#include "Include.H"
#include "Packet.H"

PacketPool::PacketPool(){
	return;
}

PacketPool::~PacketPool(){
	//Clear all saved packet memory
	/*
	for(list<Packet*>::iterator it = ++packet_pool.begin(); it != packet_pool.end(); it++){
		delete (*it);
	}
	*/
	return;
}
Packet* PacketPool::AddPacket(Packet* pkt){
	packet_pool.push_back(pkt);
	pkt->SetPacketPoolIt(--packet_pool.end());
	return pkt;

}

void PacketPool::ShowPackets(){
	for(list<Packet*>::iterator it = packet_pool.begin(); it!=packet_pool.end(); it++){
		(*it)->Show();
	}
}

list<Packet*>::iterator PacketPool::GetPacketFirstIt(){
	return packet_pool.begin();
}

list<Packet*>::iterator PacketPool::GetPacketLastIt(){
	return packet_pool.end();
}

list<Packet*>::iterator PacketPool::RemovePacketIt(list<Packet*>::iterator it){
		packet_pool.erase(it);
		return it;
}

