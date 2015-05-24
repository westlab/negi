/*
 * Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 *
 * Please see the LICENSE file distributed with this source
 * code archive for information covering the modification and
 * redistribution of this file and binaries built from it.
 */

/*
 * $Id: TcpConn.H,v 5.5 2012/08/23 04:04:23 sin Exp $
*/
#ifndef TCP_CONN_H_
#define TCP_CONN_H_
#pragma once
#include "include.h"
#include "packet.h"
#include "stream.h"
class Stream;
class Packet;


class TcpConn{
 private:
        struct in6_addr src_ip_, dst_ip_;  // Source/Destination IP address
        u_int src_port_, dst_port_;  // Source/Destination Port
        struct timeval timestamp_;  // first packet reach time
        struct timeval last_updated_time_;  // last packet reach time(used for gabage collection)
        list<Stream *> stream_list_;  // list of *packet belong to this stream
        // last stream's last packet's direction(used for stream change detection)
        u_char prev_direction_;
        State state_;  // TcpConnection state(START,CONTINUE,FINISHED)
        multimap<u_int, TcpConn*>::iterator tcp_conn_pool_it_;  // used for gabage collection
        bool destructing_flag_;  // used for gabage collection

 public:
        TcpConn(unsigned int port_map_key, Packet * pkt);
        ~TcpConn();
        void AddStream(Stream * stream);
        struct in6_addr GetSrcIP() {return src_ip_;}
        struct in6_addr GetDstIP() {return dst_ip_;}
        u_int GetSrcPort() {return src_port_;}
        u_int GetDstPort() {return dst_port_;}
        void SetState(State st) {state_ = st;}
        State GetState() {return state_;}
        void SetPrevDirection(u_char dir);
        u_char GetPrevDirection() {return prev_direction_;}
        int GetStreamSize() {return stream_list_.size();}
        list<Stream*>::iterator GetStreamFirstIt();
        list<Stream*>::iterator GetStreamLastIt();
        list<Stream*>::iterator RemoveStreamIt(list<Stream*>::iterator it);
        struct timeval GetLastUpdatedTime() {return last_updated_time_;}

        multimap<u_int, TcpConn*>::iterator GetTcpConnPoolIt() {return tcp_conn_pool_it_;}
        void SetTcpConnPoolIt(multimap<u_int, TcpConn*>::iterator it) {tcp_conn_pool_it_ = it;}
        void Show();
};
#endif  // TCP_CONN_H_
