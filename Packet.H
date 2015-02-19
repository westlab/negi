/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Packet.H,v 5.9 2012/08/23 04:04:07 sin Exp $
*/
#pragma once
#include "Include.H"
#include "Stream.H"
class Stream;

#ifdef PACKET_DEBUG_EN
#define PACKET_DEBUG(x)  x ;
#else
#define PACKET_DEBUG(x)   ;
#endif


typedef struct PacketCnt_t{
	struct pcap_pkthdr pcap_hdr;
	u_char pcap_pkt[];
}PacketCnt;
	


class Packet
{
	private:
		PacketCnt *packet_cnt;			// pointer to libpcap packet structure(w/ header) 
		Stream *stream;					// pointer to stream that packet belong to
		unsigned char* packet;			// ponter to packet binary
		struct ether_header* eth_header;	// pointer to Ether header structure
		struct vlan_ethhdr* vlan_eth_header;	// pointer to VLAN tagged Ether header structure
		struct iphdr* ip_header;			// pointer to IP header structure
		struct ip6_hdr* ip6_header;			// pointer to IP header structure
		struct tcphdr* tcp_header;		// pointer to TCP header structure 
		unsigned int l2_header_size;	// L2(MAC) header size
		unsigned int ether_proto;

		string src_mac_addr;
		string dst_mac_addr;

		bool vlan_tag_flag;				//TRUE if vlan tagged

		unsigned int l3_header_size;	// L3(IP) header size
		unsigned int l4_header_size;	// L4(TCP/UDP) header size
		unsigned int packet_size;		// packet size (caliculated from IP header) <- nomally use this!
		unsigned int packet_size_cap;	// captured packet size(returned by libpcap)
		unsigned int packet_size_org;	// original packet size(returned by libpcap)
		unsigned int content_size;		// payload size
		unsigned int l7_content_size;	// Layer 7 content size(with HTTP/1.1 decode, etc)
		unsigned int version;			// Packet IP Version(4 or 6)

		unsigned char* l3_header;		// pointer to L3(MAC) header
		unsigned char* l4_header;		// pointer to L4(TCP/UDP) header
		unsigned char* content;			// pointer to payload begin
		unsigned char* l7_content;		// pointer to Layer7 content begin

		struct timeval	timestamp;		// Packet reached timestamp(returnd by libpcap)

		unsigned int protocol;			// Transport Protocol(ex: TCP:6) (in IPv6: Next Header)
		bool ack;						// TCP ACK flag
		bool fin;						// TCP FIN flag
		bool syn;						// TCP SYN flag
		bool urg;						// TCP URG flag
		bool psh;						// TCP PUSH flag
		bool rst;						// TCP RESET flag

		struct in6_addr src_ip;			// Source IP(IPv4 included v6 with ::ffff:x.x.x.x)
		struct in6_addr dst_ip;			// Destination IP
		
		char src_ip_str[INET6_ADDRSTRLEN];
		char dst_ip_str[INET6_ADDRSTRLEN];

		unsigned int src_port;			// Source Port
		unsigned int dst_port;			// Destination Port

		unsigned int seq_no;			// TCP sequence number

		size_t mem_size;				// not used.

		list<Packet*>::iterator packet_pool_it;			// used for gabage collect
		list<Packet*>::iterator stream_packet_list_it;	// used for gabage collect

		int error;						// error number


	public:
		Packet(PacketCnt *pcnt);
		Packet(string timestamp_str, string content_size_str, string src_ip_str, string src_port_str, string dst_ip_str, string dst_port_str, string flag_str, string content_str);
		~Packet();
		void SetStream(Stream *p_stream);
		Stream *GetStream();
		unsigned char *GetPacket();
		PacketCnt *GetPacketContainer(){return packet_cnt;}
		unsigned char *GetL3Header();
		unsigned char *GetL4Header();
		string GetSrcMacAddr(){return src_mac_addr;}
		string GetDstMacAddr(){return dst_mac_addr;}
		struct in6_addr GetSrcIP(){return src_ip;}
		struct in6_addr GetDstIP(){return dst_ip;}
		char* GetSrcIPStr(){return src_ip_str;}
		char* GetDstIPStr(){return dst_ip_str;}
		unsigned int GetSrcPort(){return src_port;}
		unsigned int GetDstPort(){return dst_port;}
		unsigned int GetSeqNo(){return seq_no;}
		unsigned char *GetContent();
		unsigned int GetPacketSize(){return packet_size;}
		unsigned int GetPacketSizeOrg(){return packet_size_org;}
		unsigned char * GetL7Content(){return l7_content;}
		void SetL7Content(unsigned char *l7c){l7_content = l7c;}
		unsigned int GetContentSize(){return content_size;}
		void SetL7ContentSize(unsigned int size);
		u_int GetL7ContentSize(){return l7_content_size;}
		struct timeval GetTimestamp();

		u_int GetProtocol(){return protocol;}
		bool GetAck(){return ack;}
		bool GetFin(){return fin;}
		bool GetSyn(){return syn;}
		bool GetUrg(){return urg;}
		bool GetPsh(){return psh;}
		bool GetRst(){return rst;}
		
		int ExFlag(){return (fin || syn || urg || psh || rst);}

		list<Packet*>::iterator GetPacketPoolIt(){return packet_pool_it;}
		void SetPacketPoolIt(list<Packet*>::iterator it){packet_pool_it = it;}
		list<Packet*>::iterator GetStreamIt(){return stream_packet_list_it;}
		void SetStreamIt(list<Packet*>::iterator it){stream_packet_list_it = it;}

		void SetError(){error = 1;}
		int GetError(){return error;}

		void ShowContent();
		void Show();


		//for Test.C
		void SetContentSize(u_int size);
};
