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

#include "packet_pool.h"
#include "include.h"
#include "packet.h"


PacketPool::PacketPool(){
    return;
}

PacketPool::~PacketPool(){
    return;
}
Packet* PacketPool::AddPacket(Packet* pkt){
    packet_pool_.push_back(pkt);
    pkt->SetPacketPoolIt(--packet_pool_.end());
    return pkt;

}

void PacketPool::ShowPackets(){
    for(list<Packet*>::iterator it = packet_pool_.begin(); it!=packet_pool_.end(); it++){
        (*it)->Show();
    }
}

list<Packet*>::iterator PacketPool::GetPacketFirstIt(){
    return packet_pool_.begin();
}

list<Packet*>::iterator PacketPool::GetPacketLastIt(){
    return packet_pool_.end();
}

list<Packet*>::iterator PacketPool::RemovePacketIt(list<Packet*>::iterator it){
        packet_pool_.erase(it);
        return it;
}
