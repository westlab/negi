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

#include "include.h"
#include "global.h"
#include "stream_rebuild.h"
#include "glog/logging.h"

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

    LOG(INFO) << "[Packet]------------------" ;
    //find map using src or dst port

    unsigned int port_map_key;

    if(pkt->GetSrcPort() > pkt->GetDstPort()){
        //use Bigger port number to connection map key.
        port_map_key = pkt->GetSrcPort();
    }else{
        port_map_key = pkt->GetDstPort();
    }

    LOG(INFO) << "Key value is: "<< port_map_key ;

    TcpConn *tcp_conn;
    Stream *stream;

    if(pkt->GetSyn() && !pkt->GetAck()){
        LOG(INFO) << "SYN detected. New tcp connection entry created." ;
        //If packet containes SYN, create new stream entry.
        tcp_conn = new TcpConn(port_map_key, pkt);
        return;

    }else{
        if(!pkt->GetFin() && !pkt->GetContentSize()){
            LOG(INFO) << "No Container. Discard." ;
            return;
        }
        unsigned int tcp_conn_num = tcp_conn_pool->GetTcpConnCount(port_map_key);
        LOG(INFO) << "Map found : "<<tcp_conn_num <<" entries." ;

        if(tcp_conn_num == 0){
            //Something err.
            LOG(INFO) <<"ERROR! No match tcp connection found!!" ;
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
                        LOG(INFO) << "Entry matched! Client -> Server" ;


                        switch(tcp_conn->GetPrevDirection()){
                            case NEW:
                                LOG(INFO) << "New connection data arrived. create stream, Set prev_direction to C2S." ;
                                stream = new Stream(pkt);
                                stream->AddPacket(pkt);
                                stream->SetDirection(C2S);
                                tcp_conn->AddStream(stream);
                                tcp_conn->SetPrevDirection(C2S);
                                break;

                            case C2S:
                                LOG(INFO) << "Stream continueing. add data to stream." ;
                                //if direction is same as previous packet, add packet to stream.
                                (*--(tcp_conn->GetStreamLastIt()))->AddPacket(pkt);
                                (*--(tcp_conn->GetStreamLastIt()))->SetState(CONTINUE);
                                break;
                            case S2C:
                                LOG(INFO) << "Direction changed. Create new stream." ;
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
                            LOG(INFO) << "FIN flag detected. connection state to FINISHED. tcp connection entry." ;
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
                            LOG(INFO) << "RST flag detected. connection state to FINISHED. tcp connection entry." ;
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
                        LOG(INFO) << "Entry matched! Server -> Client" ;


                        switch(tcp_conn->GetPrevDirection()){
                            case NEW:
                                LOG(INFO) << "New connection data arrived. create stream, Set prev_direction to S2C." ;

                                stream = new Stream(pkt);
                                stream->AddPacket(pkt);
                                stream->SetDirection(S2C);
                                tcp_conn->AddStream(stream);
                                tcp_conn->SetPrevDirection(S2C);
                                break;

                            case S2C:
                                LOG(INFO) << "Stream continueing. add data to stream." ;
                                //if direction is same as previous packet, add packet to stream.
                                (*--(tcp_conn->GetStreamLastIt()))->AddPacket(pkt);
                                (*--(tcp_conn->GetStreamLastIt()))->SetState(CONTINUE);
                                break;

                            case C2S:
                                LOG(INFO) << "Direction changed. Create new stream." ;
                                Stream * last_stream = *--(tcp_conn->GetStreamLastIt());
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
                            LOG(INFO) << "FIN flag detected. delete tcp connection entry." ;
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
                            LOG(INFO) << "RST flag detected. delete tcp connection entry." ;
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
