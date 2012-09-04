/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Observer.H,v 5.3 2012-04-30 09:42:07 sin Exp $
*/
#pragma once
#include "Include.H"
#include "Observer.H"
#include "Packet.H"

#ifdef OBSERVER_DEBUG_EN
#define OBSERVER_DEBUG(x)  x ;
#else
#define OBSERVER_DEBUG(x)   ;
#endif


class Observer
{
	private:
		unsigned long long num_packet_created;
		unsigned long long num_packet_deleted;
		unsigned long long num_stream_created;
		unsigned long long num_stream_deleted;
		unsigned long long num_tcpconn_created;
		unsigned long long num_tcpconn_deleted;
		unsigned long long mpf_mem_size;
		unsigned long long num_stream_disorderd;
		timeval last_timestamp;
		bool flag_same_time;
		timeval offset_time;

		unsigned long long num_packet_mallocd;
		unsigned long long num_packet_freed;
		unsigned long long num_packet_mem_inuse;

		unsigned long long stream_mem_mallocd;
		unsigned long long stream_mem_freed;
		unsigned long long stream_mem_inuse;

		unsigned long long sim_stream_mode_mem_inuse;


		unsigned long long mpf_mem_inuse;
///*
		unsigned long long mpf_activerule_mallocd;
		unsigned long long mpf_activerule_freed;
		unsigned long long mpf_activerule_mem_inuse;
		unsigned long long mpf_tempbuf_mallocd;
		unsigned long long mpf_tempbuf_freed;
		unsigned long long mpf_tempbuf_mem_inuse;
		unsigned long long mpf_state_mallocd;
		unsigned long long mpf_state_freed;
		unsigned long long mpf_state_mem_inuse;
//*/

	public:
		Observer();
		~Observer();
		void SetDefault();
		void Show();
		void ShowMem(timeval timestamp);
		void Proc(Packet *pkt);
		void PacketCreated();
		void PacketDeleted();
		void StreamCreated();
		void StreamDeleted();
		void StreamDisorderd();
		void StreamDisorderRecoverd();
		void TcpConnCreated();
		void TcpConnDeleted();
		void PacketMallocd(size_t size);
		void PacketFreed(size_t size);
		void StreamMallocd(size_t size);
		void StreamFreed(size_t size, size_t streampktsize);
		void SimPacketMallocd(size_t size);

		void MPFActiveRuleMallocd(size_t size);
		void MPFActiveRuleDeleted(size_t size);
		void MPFTempBufMallocd(size_t size);
		void MPFTempBufDeleted(size_t size);
		void MPFStateMallocd(size_t size);
		void MPFStateDeleted(size_t size);

		u_long http_stream_byte_sum; 
		u_long http_stream_packet_sum; 
		u_long http_stream_num; 

		u_long http_gzip_stream_byte_sum; 
		u_long http_gzip_stream_packet_sum; 
		u_long http_gzip_stream_num; 
};
