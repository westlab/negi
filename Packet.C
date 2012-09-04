/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Packet.C,v 5.13 2012-08-22 06:17:48 sin Exp $
*/
#include "Packet.H"
#include "Global.H"

Packet::Packet(PacketCnt *pcnt){

	packet_cnt = pcnt;
	stream = NULL; //default
	l7_content = NULL; //default
	timestamp = packet_cnt->pcap_hdr.ts;
	packet_size_cap = packet_cnt->pcap_hdr.caplen;
	packet_size_org = packet_cnt->pcap_hdr.len;

	error = 0;

	packet = packet_cnt->pcap_pkt;

	l2_header_size = sizeof(struct ether_header);
	l3_header = packet + sizeof(struct ether_header); //IP header
	ip_header = (struct ip *)l3_header;
	src_ip = ip_header->ip_src;
	dst_ip = ip_header->ip_dst;
	protocol = ip_header->ip_p;

	l3_header_size = ip_header->ip_hl*4;

	l4_header = l3_header + ip_header->ip_hl*4; //TCP/UDP header
	packet_size = static_cast<unsigned int>(ntohs(ip_header->ip_len)) + l2_header_size;

	if(protocol == IPPROTO_TCP){
		PACKET_DEBUG(RED cout << "TCP Packet!" << endl ;RESET);
		tcp_header = (struct tcphdr *)l4_header;
		src_port = ntohs(tcp_header->source);
		dst_port = ntohs(tcp_header->dest);
		seq_no = ntohl(tcp_header->seq);
		ack = tcp_header->ack;
		fin = tcp_header->fin;
		syn = tcp_header->syn;
		urg = tcp_header->urg;
		psh = tcp_header->psh;
		rst = tcp_header->rst;
		l4_header_size = tcp_header->doff*4;
		content_size = packet_size - l2_header_size - l3_header_size - l4_header_size;
	} else if(protocol == IPPROTO_UDP){
		PACKET_DEBUG(RED cout << "UDP Packet!" << endl ;RESET);
		struct udphdr* udp_header = (struct udphdr *)l4_header;
		src_port = ntohs(udp_header->source);
		dst_port = ntohs(udp_header->dest);
		l4_header_size = ntohs(udp_header->len);
		content_size = packet_size - ip_header->ip_hl*4 - sizeof(struct udphdr);
	} else{
	PACKET_DEBUG(RED	cout << "This is not TCP/UDP packet!!" <<endl; RESET);
		src_port = 0;
		dst_port = 0;
		content_size = 0;
	}
	if(content_size > packet_size_cap){
		content_size = 0;
	}

	content = l4_header + l4_header_size; //caliculate start iterator of content
	l7_content = content;
	l7_content_size = content_size;

	//Add to packet_pool
//	packet_pool.AddPacket(this);

	if(packet_size_cap < packet_size){
//		Show();
	}
	
	return;
}

Packet::Packet(string timestamp_str, string content_size_str, string srcip_str, string src_port_str, string dstip_str, string dst_port_str, string flag_str, string content_str){
	//Count packet creation
	observer->PacketCreated();
	

	mem_size = atoi(content_size_str.c_str()) +1;
	packet = (u_char *)malloc(mem_size);
	memcpy(packet, content_str.c_str(), mem_size);

	//Count packet creation
	observer->PacketMallocd(mem_size);

	src_port = atoi(src_port_str.c_str());
	dst_port = atoi(dst_port_str.c_str());

	inet_aton(srcip_str.c_str(),&src_ip);
	inet_aton(dstip_str.c_str(),&dst_ip);

	stream = 0; //default
//	gettimeofday(&timestamp,0);
	timestamp.tv_sec = atoi(timestamp_str.c_str());
	timestamp.tv_usec = 0;
	packet_size = mem_size;
	packet_size_org = mem_size;

	protocol = IPPROTO_TCP;

	if(flag_str == "ACK"){
		ack = 1;
		syn = fin = urg = psh = rst = 0;
	}
	if(flag_str == "SYN"){
		syn = 1;
		ack = fin = urg = psh = rst = 0;
	}
	if(flag_str == "FIN"){ 
		fin = 1;
		ack = syn = urg = psh = rst = 0;
	}

	content_size = mem_size;
	content = packet; 

	//Add to packet_pool
	packet_pool->AddPacket(this);
	
	return;
}

Packet::~Packet(){
	//Count packet deletion
	observer->PacketDeleted();

	if(l7_content != content) free(l7_content);
	
//	packet_pool.RemovePacketIt(packet_pool_it);
//#ifndef STREAM_CLEAN_MODE
	if(savemode == PACKET){
		if(stream != NULL){
			stream->RemovePacketIt(stream_packet_list_it);
		}
	}
//#endif

	if(packet_cnt != NULL){
		//Count packet deletion
//		observer->PacketFreed(mem_size);
		free(packet_cnt);
	packet_cnt = NULL;
	//	packet_pool.ShowPackets();
	}
}

void Packet::SetStream(Stream *p_stream){
	stream = p_stream;
}

Stream* Packet::GetStream(){
	return stream;
}

unsigned char *Packet::GetL3Header(){
	return l3_header;	
}

unsigned char *Packet::GetL4Header(){
	return l4_header;	
}

unsigned char *Packet::GetContent(){
	return content;	
}

struct timeval Packet::GetTimestamp(){
	return timestamp;
}


void Packet::Show(){
	string src_ip_str, dst_ip_str;
	src_ip_str = inet_ntoa(src_ip);
	dst_ip_str = inet_ntoa(dst_ip);
	
	YELLOW
	cout << "PACKET------------------------------------" <<endl;
	cout << "Timestamp: " << timestamp.tv_sec << "." << timestamp.tv_usec <<endl;
	cout << "EtherID: " << protocol <<endl;
	cout << "IP: " << src_ip_str << ":" << src_port << " -> " << dst_ip_str << ":" << dst_port <<endl;
	cout << "Packet size: [ORG " << packet_size_org <<"] ";
	cout << "[CAP " << packet_size_cap << "] ";
	cout << "[HDR " << packet_size <<"] " << endl;

	cout << "Packet ";
	cout << "[L2 Header: " << l2_header_size << "] ";
	cout << "[L3 Header: " << l3_header_size << "] ";
	cout << "[L4 Header: " << l4_header_size << "] ";
	cout << "[Contents: " << content_size << "] " << endl;



	if(protocol == IPPROTO_TCP){
		cout << "TcpFlag: " ;
		if(ack) cout << "[ACK]";
		if(syn) cout << "[SYN]";
		if(fin) cout << "[FIN]";
		if(urg) cout << "[URG]";
		if(psh) cout << "[PSH]";
		if(rst) cout << "[RST]";
		cout << endl; RESET
		cout << "[Sequence No: " << seq_no << "] " << endl;


/*
		cout << "Content: ["; RESET
//		for(u_int i=0; i <content_size ; i++){
//			cout << *(content + i) ;
//		}
		for(u_int i=0; i <packet_size ; i++){
			cout << *(packet + i) ;
		}
		YELLOW cout << "]" <<endl; RESET
*/
	}

	return;
}
void Packet::SetL7ContentSize(unsigned int size){
	stream->SetL7RetrievedContentSize(stream->GetL7RetrievedContentSize() - GetL7ContentSize() + size);
	l7_content_size = size;
}

void Packet::ShowContent(){
		for(u_int i=0; i <content_size ; i++){
			cout << *(content + i) ;
		}

}

//for Test.C
void Packet::SetContentSize(u_int size){
	content_size = size;
	l7_content_size = size;
}
