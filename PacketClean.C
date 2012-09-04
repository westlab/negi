/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: PacketClean.C,v 5.12 2012-07-04 03:41:22 sin Exp $
*/
#include "PacketClean.H"
#include "Global.H"

PacketClean::PacketClean(){
	last_gc_time.tv_sec = 0;
	last_gc_time.tv_usec = 0;

	return;
}
PacketClean::~PacketClean(){
	return;
}

void PacketClean::Proc(Packet *pkt){

	struct timeval	packet_timestamp = pkt->GetTimestamp();

	if(savemode == PACKET){
		delete(pkt);
	}else if(savemode == STREAM){
		//if packet has no parent stream, delete it
		if(pkt->GetStream() == NULL){
			delete(pkt);
		}
	}

	//Stream End
	if(!end_stream_list.empty()){
//			cout << "end_stream_list size is :" << end_stream_list.size() << endl;
		for(list<Stream *>::iterator it=end_stream_list.begin(); it != end_stream_list.end(); it++){
				delete(*it);
		//	}
		}
		end_stream_list.clear();
	}


		//Gabage collect is enough 1 sec. each.
		if(packet_timestamp.tv_sec - last_gc_time.tv_sec >= 1){


			//Check all stream for timeout.
			for(list<Stream *>::iterator it=stream_pool->GetStreamFirstIt(); it != stream_pool->GetStreamLastIt(); it++){
				if( packet_timestamp.tv_sec - (*it)->GetLastUpdatedTime().tv_sec >= gc_remove_time){
					list<Stream *>::iterator it2;
					it2 = --it;
					++it;
					if(it != stream_pool->GetStreamLastIt()){
						delete *it;
					}
					it = it2;
				}
			}

			//Check all tcpconn for timeout.
			for(multimap<u_int, TcpConn*>::iterator it=tcp_conn_pool->GetTcpConnFirstIt(); it != tcp_conn_pool->GetTcpConnLastIt(); ++it){
				if( packet_timestamp.tv_sec - (it->second)->GetLastUpdatedTime().tv_sec >= gc_remove_time){
					multimap<u_int, TcpConn*>::iterator it2;
						it2 = --it;
						++it;
					if(it != tcp_conn_pool->GetTcpConnLastIt()){
				//cout << "TcpConn removing!!"<< endl;
						delete it->second;
					}
					it = it2;
				}
			}

			last_gc_time.tv_sec = packet_timestamp.tv_sec;
		}
//		cout << "stream num :"<<stream_pool.GetSize()<< endl;
}
