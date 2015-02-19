/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: TcpConn.H,v 5.5 2012/08/23 04:04:23 sin Exp $
*/
#pragma once
#include "Include.H"
#include "Packet.H"
#include "Stream.H"
class Stream;
class Packet;


class TcpConn{
	private:
		struct in6_addr src_ip,dst_ip;							// Source/Destination IP address
		u_int src_port,dst_port;								// Source/Destination Port
		struct timeval timestamp;								// first packet reach time
		struct timeval last_updated_time;						// last packet reach time(used for gabage collection)
		list<Stream *> stream_list;								// list of *packet belong to this stream
		u_char prev_direction;									// last stream's last packet's direction(used for stream change detection)
		State state;											// TcpConnection state(START,CONTINUE,FINISHED)
		multimap<u_int, TcpConn*>::iterator tcp_conn_pool_it;	//used for gabage collection
		bool destructing_flag;									//used for gabage collection

	public:
		TcpConn(unsigned int port_map_key, Packet * pkt);
		~TcpConn();
		void AddStream(Stream * stream);
		struct in6_addr GetSrcIP(){return src_ip;}
		struct in6_addr GetDstIP(){return dst_ip;}
		u_int GetSrcPort(){return src_port;}
		u_int GetDstPort(){return dst_port;}
		void SetState(State st){state = st;}
		State GetState(){return state;}
		void SetPrevDirection(u_char dir);
		u_char GetPrevDirection(){return prev_direction;}
		int GetStreamSize(){ return stream_list.size();}
		list<Stream*>::iterator GetStreamFirstIt();
		list<Stream*>::iterator GetStreamLastIt(); 
		list<Stream*>::iterator RemoveStreamIt(list<Stream*>::iterator it); 
		struct timeval GetLastUpdatedTime(){return last_updated_time;};

		multimap<u_int, TcpConn*>::iterator GetTcpConnPoolIt(){return tcp_conn_pool_it;}
		void SetTcpConnPoolIt(multimap<u_int, TcpConn*>::iterator it){tcp_conn_pool_it = it;}
		void Show();
};
