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
#include "packet.h"
#include "global.h"
#include "glog/logging.h"

Packet::Packet(PacketCnt *pcnt){

    packet_cnt_ = pcnt;
    stream_ = NULL; //default
    l7_content_ = NULL; //default
    timestamp_ = packet_cnt_->pcap_hdr.ts;
    packet_size_cap_ = packet_cnt_->pcap_hdr.caplen;
    packet_size_org_ = packet_cnt_->pcap_hdr.len;

    error_ = 0;

    packet_ = (unsigned char *)malloc(packet_size_cap_);
    memcpy(packet_, packet_cnt_->pcap_pkt, packet_size_cap_);

    l2_header_size_ = sizeof(struct ether_header);
    eth_header_ = (struct ether_header *) packet_;

    stringstream ss;
    ss.str("");
    ss.clear(stringstream::goodbit);
    for(int i = 0; i < 5; i++){
        ss << hex << setw(2) << setfill('0') <<  (int)eth_header_->ether_shost[i] << ":";
    }
    ss << hex << setw(2) << setfill('0') << (int)eth_header_->ether_shost[5];
    src_mac_addr_ = ss.str();

    ss.str("");
    ss.clear(stringstream::goodbit);
    for(int i = 0; i < 5; i++){
        ss << hex << setw(2) << setfill('0') <<  (int)eth_header_->ether_dhost[i] << ":";
    }
    ss << hex << setw(2) << setfill('0') << (int)eth_header_->ether_dhost[5];
    dst_mac_addr_ = ss.str();


    if(ntohs(eth_header_->ether_type) == ETH_P_8021Q){
        l3_header_ = packet_ + sizeof(struct vlan_ethhdr);
        vlan_eth_header_ = (struct vlan_ethhdr *) packet_;
        ether_proto_ = ntohs(vlan_eth_header_->ether_type);
    }else{
        l3_header_ = packet_ + sizeof(struct ether_header); //IP header
        ether_proto_ = ntohs(eth_header_->ether_type);
    }

    switch (ether_proto_){
        case ETH_P_IP:
            version_ = 4;
            ip_header_ = (struct iphdr *)l3_header_;
            inet_v4tov6((struct in_addr *)(&(ip_header_->saddr)), &src_ip_);
            inet_v4tov6((struct in_addr *)(&(ip_header_->daddr)), &dst_ip_);
            protocol_ = ip_header_->protocol;

            l3_header_size_ = ip_header_->ihl*4;

            l4_header_ = l3_header_ + ip_header_->ihl*4; //TCP/UDP header
            packet_size_ = static_cast<unsigned int>(ntohs(ip_header_->tot_len)) + l2_header_size_;

            struct in_addr v4_src_ip ,v4_dst_ip;
            inet_v6tov4(&src_ip_ , &v4_src_ip);
            inet_v6tov4(&dst_ip_ , &v4_dst_ip);

            inet_ntop(AF_INET, &v4_src_ip, src_ip_str_, INET6_ADDRSTRLEN);
            inet_ntop(AF_INET, &v4_dst_ip, dst_ip_str_, INET6_ADDRSTRLEN);

            ip6_header_ = NULL;

            break;

        case ETH_P_IPV6:
            version_ = 6;
            ip6_header_ = (struct ip6_hdr *)l3_header_;
            src_ip_ = ip6_header_->ip6_src;
            dst_ip_ = ip6_header_->ip6_dst;
            protocol_ = ip6_header_->ip6_ctlun.ip6_un1.ip6_un1_nxt;

            l3_header_size_ = 40;	//FIXED Size. no follow about extension header.

            l4_header_ = l3_header_ + 40; //TCP/UDP header
            //in IPv6, IP Header size is not included in payload length(ip6_un1_plen).
            packet_size_ = static_cast<unsigned int>(ntohs(ip6_header_->ip6_ctlun.ip6_un1.ip6_un1_plen))
                           + l2_header_size_ + l3_header_size_;

            inet_ntop(AF_INET6, &src_ip_, src_ip_str_, INET6_ADDRSTRLEN);
            inet_ntop(AF_INET6, &dst_ip_, dst_ip_str_, INET6_ADDRSTRLEN);

            ip_header_ = NULL;
            break;

        default:
            LOG(ERROR) << "This is not IPv4 not IPv6";
            version_ = 0;
            src_port_ = 0;
            dst_port_ = 0;
            content_size_ = 0;
            error_ = 1;
            return;
    }


    if(protocol_ == IPPROTO_TCP){
        LOG(INFO) << "TCP Packet";
        tcp_header_ = (struct tcphdr *)l4_header_;
        src_port_ = ntohs(tcp_header_->source);
        dst_port_ = ntohs(tcp_header_->dest);
        seq_no_ = ntohl(tcp_header_->seq);
        ack_ = tcp_header_->ack;
        fin_ = tcp_header_->fin;
        syn_ = tcp_header_->syn;
        urg_ = tcp_header_->urg;
        psh_ = tcp_header_->psh;
        rst_ = tcp_header_->rst;
        l4_header_size_ = tcp_header_->doff*4;
        content_size_ = packet_size_ - l2_header_size_ - l3_header_size_ - l4_header_size_;


    } else if(protocol_ == IPPROTO_UDP){
        LOG(INFO) << "UCP Packet";
        struct udphdr* udp_header = (struct udphdr *)l4_header_;
        src_port_ = ntohs(udp_header->source);
        dst_port_ = ntohs(udp_header->dest);
        l4_header_size_ = ntohs(udp_header->len);
        content_size_ = packet_size_ - l3_header_size_ - sizeof(struct udphdr);
    } else{
        src_port_ = 0;
        dst_port_ = 0;
        content_size_ = 0;
    }
    if(content_size_ > packet_size_cap_){
        content_size_ = 0;
    }

    content_ = l4_header_ + l4_header_size_; //caliculate start iterator of content
    l7_content_ = content_;
    l7_content_size_ = content_size_;

    if(packet_size_cap_ < packet_size_){
        //Show();
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

    return;
}
*/

Packet::~Packet(){
    //Count packet deletion
    observer->PacketDeleted();

    if(savemode == PACKET){
        if(stream_ != NULL){
            stream_->RemovePacketIt(stream_packet_list_it_);
        }
    }

    if(packet_ != NULL){
    free(packet_);
    packet_ = NULL;
    }
}

void Packet::SetStream(Stream *p_stream){
    stream_ = p_stream;
}

Stream* Packet::GetStream(){
    return stream_;
}

unsigned char *Packet::GetL3Header(){
    return l3_header_;
}

unsigned char *Packet::GetL4Header(){
    return l4_header_;
}

unsigned char *Packet::GetContent(){
    return content_;
}

struct timeval Packet::GetTimestamp(){
    return timestamp_;
}


void Packet::Show(){

    YELLOW
    cout << "PACKET------------------------------------" <<endl;
    cout << "Timestamp: " << timestamp_.tv_sec << "." << timestamp_.tv_usec <<endl;

    cout << "VLAN Status: ";
    if(vlan_tag_flag_){
        cout << "tagged!" ;
    }else {cout << "no tag.";
    }
    cout << endl;

    cout << "EtherID: " << protocol_ <<endl;
    cout << "IP: " << src_ip_str_ << ":" << src_port_ << " -> " << dst_ip_str_ << ":" << dst_port_ <<endl;
    cout << "Packet size: [ORG " << packet_size_org_ <<"] ";
    cout << "[CAP " << packet_size_cap_ << "] ";
    cout << "[HDR " << packet_size_ <<"] " << endl;

    cout << "Packet ";
    cout << "[L2 Header size: " << l2_header_size_ << "] ";
    cout << "[L3 Header size: " << l3_header_size_ << "] ";
    cout << "[L4 Header size: " << l4_header_size_ << "] ";
    cout << "[Contents: " << content_size_ << "] " << endl;

    if(protocol_ == IPPROTO_TCP){
        cout << "TcpFlag: " ;
        if(ack_) cout << "[ACK]";
        if(syn_) cout << "[SYN]";
        if(fin_) cout << "[FIN]";
        if(urg_) cout << "[URG]";
        if(psh_) cout << "[PSH]";
        if(rst_) cout << "[RST]";
        cout << endl; RESET
        cout << "[Sequence No: " << seq_no_ << "] " << endl;
    }

    return;
}
void Packet::SetL7ContentSize(unsigned int size){
    stream_->SetL7RetrievedContentSize(stream_->GetL7RetrievedContentSize() - GetL7ContentSize() + size);
    l7_content_size_ = size;
}

void Packet::ShowContent(){
        for(u_int i=0; i < content_size_ ; i++){
            cout << *(content_ + i) ;
        }

}

//for Test.C
void Packet::SetContentSize(u_int size){
    content_size_ = size;
    l7_content_size_ = size;
}
