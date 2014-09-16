/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: StreamRebuild.C,v 5.3 2011-11-14 07:05:26 sin Exp $
*/

#include "Include.H"
#include "Global.H"
#include "StreamRebuild.H"

StreamRebuild::StreamRebuild(){
	return;
}

void StreamRebuild::Proc(Packet *pkt){
	if(pkt->GetProtocol() == IPPROTO_TCP){
		SearchStream(pkt);
	}else{ }

}
void StreamRebuild::Init(Packet *pkt){
}

void StreamRebuild::SearchStream(Packet * pkt){

	ST_REBUILD_DEBUG(BLUE cout << "[Packet]------------------" << endl; RESET);
	//find map using src or dst port 
	
	unsigned int port_map_key;
	
	/*
	unsigned int uint_srcip = (unsigned int)((pkt->GetSrcIP()).s_addr);
	unsigned int uint_dstip = (unsigned int)((pkt->GetDstIP()).s_addr);

	if(uint_srcip > uint_dstip){
		//use Bigger port number to connection map key.
		port_map_key = uint_srcip;
	}else{
		port_map_key = uint_dstip;
	}
	*/

	if(pkt->GetSrcPort() > pkt->GetDstPort()){
		//use Bigger port number to connection map key.
		port_map_key = pkt->GetSrcPort();
	}else{
		port_map_key = pkt->GetDstPort();
	}

	ST_REBUILD_DEBUG(BLUE cout << "Key value is: "<< port_map_key << endl; RESET);
	
	TcpConn *tcp_conn;
	Stream *stream;

	if(pkt->GetSyn() && !pkt->GetAck()){
		ST_REBUILD_DEBUG( BLUE cout << "SYN detected. New tcp connection entry created." << endl; RESET);
		//If packet containes SYN, create new stream entry.
		tcp_conn = new TcpConn(port_map_key, pkt);
		return;

	}else{
		if(!pkt->GetFin() && !pkt->GetContentSize()){
			ST_REBUILD_DEBUG( BLUE cout << "No Container. Discard." << endl; RESET);
			return;
		}
		unsigned int tcp_conn_num = tcp_conn_pool->GetTcpConnCount(port_map_key);
		ST_REBUILD_DEBUG( BLUE cout << "Map found : "<<tcp_conn_num <<" entries." << endl; RESET);

		if(tcp_conn_num == 0){
			//Something err.
			ST_REBUILD_DEBUG( RED cout <<"ERROR! No match tcp connection found!!" << endl; RESET);
			return;
		}else {
			multimap<unsigned int, TcpConn *>::iterator tcp_conn_map_it = tcp_conn_pool->FindTcpConn(port_map_key);
			for(u_int i=0; i < tcp_conn_num; i++, tcp_conn_map_it++){
				tcp_conn = tcp_conn_map_it->second;

				struct in6_addr packet_src_ip, packet_dst_ip;
				struct in6_addr tcpconn_src_ip, tcpconn_dst_ip;

				packet_src_ip = pkt->GetSrcIP();
				packet_dst_ip = pkt->GetDstIP();
				tcpconn_src_ip = tcp_conn->GetSrcIP();
				tcpconn_dst_ip = tcp_conn->GetDstIP();

				if(pkt->GetSrcPort() == tcp_conn->GetSrcPort() && pkt->GetDstPort() == tcp_conn->GetDstPort()){
					if(!memcmp(&packet_src_ip, &tcpconn_src_ip, sizeof(struct in6_addr)) &&	\
						!memcmp(&packet_dst_ip, &tcpconn_dst_ip, sizeof(struct in6_addr))){
						ST_REBUILD_DEBUG( BLUE cout << "Entry matched! Client -> Server" << endl; RESET);


						switch(tcp_conn->GetPrevDirection()){
							case NEW:
								ST_REBUILD_DEBUG( BLUE cout << "New connection data arrived. create stream, Set prev_direction to C2S." << endl; RESET);
								stream = new Stream(pkt);
								stream->AddPacket(pkt);
								stream->SetDirection(C2S);
								tcp_conn->AddStream(stream);
								tcp_conn->SetPrevDirection(C2S);
								break;

							case C2S:
								ST_REBUILD_DEBUG( BLUE cout << "Stream continueing. add data to stream." << endl; RESET);
								//if direction is same as previous packet, add packet to stream.
								(*--(tcp_conn->GetStreamLastIt()))->AddPacket(pkt);
								(*--(tcp_conn->GetStreamLastIt()))->SetState(CONTINUE);
								break;
							case S2C:
								ST_REBUILD_DEBUG( BLUE cout << "Direction changed. Create new stream." << endl; RESET);
								Stream * last_stream = *--(tcp_conn->GetStreamLastIt());
								last_stream->SetState(END);
								end_stream_list.push_back(last_stream);

								stream = new Stream(pkt);
								stream->AddPacket(pkt);
								stream->SetDirection(C2S);
								tcp_conn->AddStream(stream);
								tcp_conn->SetPrevDirection(C2S);
							
								return;
						}
						if(pkt->GetFin()){
							ST_REBUILD_DEBUG( BLUE cout << "FIN flag detected. connection state to FINISHED. tcp connection entry." << endl; RESET);
							Stream * last_stream = *--(tcp_conn->GetStreamLastIt());
							if(last_stream == NULL){return;}
							if(tcp_conn->GetStreamLastIt() == tcp_conn->GetStreamFirstIt()){return;}
							if(last_stream->GetState() == END){return;}
							last_stream->SetState(END);
							end_stream_list.push_back(last_stream);
							tcp_conn->SetState(END);
							return;
						}
						if(pkt->GetRst()){
							ST_REBUILD_DEBUG( BLUE cout << "RST flag detected. connection state to FINISHED. tcp connection entry." << endl; RESET);
							Stream * last_stream = *--(tcp_conn->GetStreamLastIt());
							if(last_stream == NULL){return;}
							if(tcp_conn->GetStreamLastIt() == tcp_conn->GetStreamFirstIt()){return;}
							if(last_stream->GetState() == END){return;}
							last_stream->SetState(END);
							end_stream_list.push_back(last_stream);
							tcp_conn->SetState(END);
							return;
						}
					}

				}else if(pkt->GetSrcPort() == tcp_conn->GetDstPort() && pkt->GetDstPort() == tcp_conn->GetSrcPort()){
					if(!memcmp(&packet_src_ip, &tcpconn_dst_ip, sizeof(struct in6_addr)) &&	\
						!memcmp(&packet_dst_ip, &tcpconn_src_ip, sizeof(struct in6_addr))){
						ST_REBUILD_DEBUG( BLUE cout << "Entry matched! Server -> Client" << endl; RESET);


						switch(tcp_conn->GetPrevDirection()){
							case NEW:
								ST_REBUILD_DEBUG( BLUE cout << "New connection data arrived. create stream, Set prev_direction to S2C." << endl; RESET);
	
								stream = new Stream(pkt);
								stream->AddPacket(pkt);
								stream->SetDirection(S2C);
								tcp_conn->AddStream(stream);
								tcp_conn->SetPrevDirection(S2C);
								break;

							case S2C:
								ST_REBUILD_DEBUG( BLUE cout << "Stream continueing. add data to stream." << endl; RESET);
								//if direction is same as previous packet, add packet to stream.
								(*--(tcp_conn->GetStreamLastIt()))->AddPacket(pkt);
								(*--(tcp_conn->GetStreamLastIt()))->SetState(CONTINUE);
								break;

							case C2S:
								ST_REBUILD_DEBUG( BLUE cout << "Direction changed. Create new stream." << endl; RESET);
								Stream * last_stream = *--(tcp_conn->GetStreamLastIt());
								//if(last_stream->GetState() == END){break;}
								last_stream->SetState(END);
								end_stream_list.push_back(last_stream);
	
								stream = new Stream(pkt);
								stream->AddPacket(pkt);
								stream->SetDirection(S2C);
								tcp_conn->AddStream(stream);
								tcp_conn->SetPrevDirection(S2C);
								break;
						}
						if(pkt->GetFin()){
							ST_REBUILD_DEBUG( BLUE cout << "FIN flag detected. delete tcp connection entry." << endl; RESET);
							Stream * last_stream = *--(tcp_conn->GetStreamLastIt());
							if(last_stream == NULL){return;}
							if(tcp_conn->GetStreamLastIt() == tcp_conn->GetStreamFirstIt()){return;}
							if(last_stream->GetState() == END){return;}
							last_stream->SetState(END);
							end_stream_list.push_back(last_stream);
							tcp_conn->SetState(END);
							return;
						}
						if(pkt->GetRst()){
							ST_REBUILD_DEBUG( BLUE cout << "RST flag detected. delete tcp connection entry." << endl; RESET);
							Stream * last_stream = *--(tcp_conn->GetStreamLastIt());
							if(last_stream == NULL){return;}
							if(tcp_conn->GetStreamLastIt() == tcp_conn->GetStreamFirstIt()){return;}
							if(last_stream->GetState() == END){return;}
							last_stream->SetState(END);
							end_stream_list.push_back(last_stream);
							tcp_conn->SetState(END);
							return;
						}
					}
				}
			}
		}
	return;


	}
}
