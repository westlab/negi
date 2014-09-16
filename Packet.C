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

	packet = (unsigned char *)malloc(packet_size_cap);
	memcpy(packet, packet_cnt->pcap_pkt, packet_size_cap);
	
	l2_header_size = sizeof(struct ether_header);
	eth_header = (struct ether_header *) packet;

///*
	stringstream ss;
	ss.str("");
	ss.clear(stringstream::goodbit);
	for(int i = 0; i < 5; i++){
		ss << hex << setw(2) << setfill('0') <<  (int)eth_header->ether_shost[i] << ":";
	}
	ss << hex << setw(2) << setfill('0') << (int)eth_header->ether_shost[5];
	src_mac_addr = ss.str();

	ss.str("");
	ss.clear(stringstream::goodbit);
	for(int i = 0; i < 5; i++){
		ss << hex << setw(2) << setfill('0') <<  (int)eth_header->ether_dhost[i] << ":";
	}
	ss << hex << setw(2) << setfill('0') << (int)eth_header->ether_dhost[5];
	dst_mac_addr = ss.str();

//*/

	if(ntohs(eth_header->ether_type) == ETH_P_8021Q){
		l3_header = packet + sizeof(struct vlan_ethhdr);
		vlan_eth_header = (struct vlan_ethhdr *) packet;
		ether_proto = ntohs(vlan_eth_header->ether_type);
	}else{
		l3_header = packet + sizeof(struct ether_header); //IP header
		ether_proto = ntohs(eth_header->ether_type);
	}

//	Show();
	switch (ether_proto){
		case ETH_P_IP:
			PACKET_DEBUG(RED cout << "IPv4!" << endl ;RESET);
			version = 4;
			ip_header = (struct iphdr *)l3_header;
			inet_v4tov6((struct in_addr *)(&(ip_header->saddr)), &src_ip);
			inet_v4tov6((struct in_addr *)(&(ip_header->daddr)), &dst_ip);
			protocol = ip_header->protocol;

			l3_header_size = ip_header->ihl*4;

			l4_header = l3_header + ip_header->ihl*4; //TCP/UDP header
			packet_size = static_cast<unsigned int>(ntohs(ip_header->tot_len)) + l2_header_size;

			struct in_addr v4_src_ip ,v4_dst_ip;
			inet_v6tov4(&src_ip , &v4_src_ip);
			inet_v6tov4(&dst_ip , &v4_dst_ip);

			inet_ntop(AF_INET, &v4_src_ip, src_ip_str, INET6_ADDRSTRLEN);
			inet_ntop(AF_INET, &v4_dst_ip, dst_ip_str, INET6_ADDRSTRLEN);

			ip6_header = NULL;

			break;

///*
		case ETH_P_IPV6:
			PACKET_DEBUG(RED cout << "IPv6!" << endl ;RESET);
			version = 6;
			ip6_header = (struct ip6_hdr *)l3_header;
			src_ip = ip6_header->ip6_src;
			dst_ip = ip6_header->ip6_dst;
			protocol = ip6_header->ip6_ctlun.ip6_un1.ip6_un1_nxt;

			l3_header_size = 40;	//FIXED Size. no follow about extension header.

			l4_header = l3_header + 40; //TCP/UDP header
			packet_size = static_cast<unsigned int>(ntohs(ip6_header->ip6_ctlun.ip6_un1.ip6_un1_plen)) + l2_header_size + l3_header_size;	//in IPv6, IP Header size is not included in payload length(ip6_un1_plen).

			inet_ntop(AF_INET6, &src_ip, src_ip_str, INET6_ADDRSTRLEN);
			inet_ntop(AF_INET6, &dst_ip, dst_ip_str, INET6_ADDRSTRLEN);

			ip_header = NULL;
			break;
//*/

		default:
			PACKET_DEBUG(RED    cout << "This is not IPv4 or IPv6 packet!!" <<endl; RESET);
			version = 0;
			src_port = 0;
			dst_port = 0;
			content_size = 0;
			error = 1;
			return;

			break;
	}


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

		PACKET_DEBUG(Show());

	} else if(protocol == IPPROTO_UDP){
		PACKET_DEBUG(RED cout << "UDP Packet!" << endl ;RESET);
		struct udphdr* udp_header = (struct udphdr *)l4_header;
		src_port = ntohs(udp_header->source);
		dst_port = ntohs(udp_header->dest);
		l4_header_size = ntohs(udp_header->len);
		content_size = packet_size - l3_header_size - sizeof(struct udphdr);
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
}
/*
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
*/

Packet::~Packet(){
	//Count packet deletion
	observer->PacketDeleted();

//	if(l7_content != content) free(l7_content);
	
//	packet_pool.RemovePacketIt(packet_pool_it);
//#ifndef STREAM_CLEAN_MODE
	if(savemode == PACKET){
		if(stream != NULL){
			stream->RemovePacketIt(stream_packet_list_it);
		}
	}
//#endif

	if(packet != NULL){
	free(packet);
	packet = NULL;
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
	
	YELLOW
	cout << "PACKET------------------------------------" <<endl;
	cout << "Timestamp: " << timestamp.tv_sec << "." << timestamp.tv_usec <<endl;

	cout << "VLAN Status: ";
	if(vlan_tag_flag){
		cout << "tagged!" ;
	}else {cout << "no tag.";
	} 
	cout << endl;

	cout << "EtherID: " << protocol <<endl;
	cout << "IP: " << src_ip_str << ":" << src_port << " -> " << dst_ip_str << ":" << dst_port <<endl;
	cout << "Packet size: [ORG " << packet_size_org <<"] ";
	cout << "[CAP " << packet_size_cap << "] ";
	cout << "[HDR " << packet_size <<"] " << endl;

	cout << "Packet ";
	cout << "[L2 Header size: " << l2_header_size << "] ";
	cout << "[L3 Header size: " << l3_header_size << "] ";
	cout << "[L4 Header size: " << l4_header_size << "] ";
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
