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
#ifndef PACKET_H_
#define PACKET_H_
#pragma once
#include "include.h"
#include "stream.h"
class Stream;

typedef struct PacketCnt_t {
    struct pcap_pkthdr pcap_hdr;
    u_char pcap_pkt[];
}PacketCnt;



class Packet{
 private:
  PacketCnt *packet_cnt_;                // pointer to libpcap packet structure(w/ header)
  Stream *stream_;                       // pointer to stream that packet belong to
  unsigned char* packet_;                // ponter to packet binary
  struct ether_header* eth_header_;      // pointer to Ether header structure
  struct vlan_ethhdr* vlan_eth_header_;  // pointer to VLAN tagged Ether header structure
  struct iphdr* ip_header_;              // pointer to IP header structure
  struct ip6_hdr* ip6_header_;           // pointer to IP header structure
  struct tcphdr* tcp_header_;            // pointer to TCP header structure
  unsigned int l2_header_size_;          // L2(MAC) header size
  unsigned int ether_proto_;

  string src_mac_addr_;
  string dst_mac_addr_;

  bool vlan_tag_flag_;                  // TRUE if vlan tagged

  unsigned int l3_header_size_;         // L3(IP) header size
  unsigned int l4_header_size_;         // L4(TCP/UDP) header size
  unsigned int packet_size_;            // packet size (caliculated from IP header)
  unsigned int packet_size_cap_;        // captured packet size(returned by libpcap)
  unsigned int packet_size_org_;        // original packet size(returned by libpcap)
  unsigned int content_size_;           // payload size
  unsigned int l7_content_size_;        // Layer 7 content size(with HTTP/1.1 decode, etc)
  unsigned int version_;                // Packet IP Version(4 or 6)

  unsigned char* l3_header_;            // pointer to L3(MAC) header
  unsigned char* l4_header_;            // pointer to L4(TCP/UDP) header
  unsigned char* content_;              // pointer to payload begin
  unsigned char* l7_content_;           // pointer to Layer7 content begin

  struct timeval timestamp_;            // Packet reached timestamp(returnd by libpcap)

  unsigned int protocol_;               // Transport Protocol(ex: TCP:6) (in IPv6: Next Header)
  bool ack_;                            // TCP ACK flag
  bool fin_;                            // TCP FIN flag
  bool syn_;                            // TCP SYN flag
  bool urg_;                            // TCP URG flag
  bool psh_;                            // TCP PUSH flag
  bool rst_;                            // TCP RESET flag

  struct in6_addr src_ip_;              // Source IP(IPv4 included v6 with ::ffff:x.x.x.x)
  struct in6_addr dst_ip_;              // Destination IP

  char src_ip_str_[INET6_ADDRSTRLEN];
  char dst_ip_str_[INET6_ADDRSTRLEN];

  unsigned int src_port_;               // Source Port
  unsigned int dst_port_;               // Destination Port

  unsigned int seq_no_;                 // TCP sequence number

  size_t mem_size_;                     // not used.

  list<Packet*>::iterator packet_pool_it_;              // used for gabage collect
  list<Packet*>::iterator stream_packet_list_it_;       // used for gabage collect

  int error_;                                           // error number


 public:
  explicit Packet(PacketCnt *pcnt);
  Packet(string timestamp_str,
         string content_size_str,
         string src_ip_str,
         string src_port_str,
         string dst_ip_str,
         string dst_port_str,
         string flag_str,
         string content_str);
  ~Packet();
  void SetStream(Stream *p_stream);
  Stream *GetStream();
  unsigned char *GetPacket();
  PacketCnt *GetPacketContainer() {return packet_cnt_;}
  unsigned char *GetL3Header();
  unsigned char *GetL4Header();
  string GetSrcMacAddr() {return src_mac_addr_;}
  string GetDstMacAddr() {return dst_mac_addr_;}
  struct in6_addr GetSrcIP() {return src_ip_;}
  struct in6_addr GetDstIP() {return dst_ip_;}
  char* GetSrcIPStr() {return src_ip_str_;}
  char* GetDstIPStr() {return dst_ip_str_;}
  unsigned int GetSrcPort() {return src_port_;}
  unsigned int GetDstPort() {return dst_port_;}
  unsigned int GetSeqNo() {return seq_no_;}
  unsigned char *GetContent();
  unsigned int GetPacketSize() {return packet_size_;}
  unsigned int GetPacketSizeOrg() {return packet_size_org_;}
  unsigned char * GetL7Content() {return l7_content_;}
  void SetL7Content(unsigned char *l7c) {l7_content_ = l7c;}
  unsigned int GetContentSize() {return content_size_;}
  void SetL7ContentSize(unsigned int size);
  u_int GetL7ContentSize() {return l7_content_size_;}
  struct timeval GetTimestamp();

  u_int GetProtocol() {return protocol_;}
  bool GetAck() {return ack_;}
  bool GetFin() {return fin_;}
  bool GetSyn() {return syn_;}
  bool GetUrg() {return urg_;}
  bool GetPsh() {return psh_;}
  bool GetRst() {return rst_;}

  int ExFlag() {return (fin_ || syn_ || urg_ || psh_ || rst_);}

  list<Packet*>::iterator GetPacketPoolIt() {return packet_pool_it_;}
  void SetPacketPoolIt(list<Packet*>::iterator it) {packet_pool_it_ = it;}
  list<Packet*>::iterator GetStreamIt() {return stream_packet_list_it_;}
  void SetStreamIt(list<Packet*>::iterator it) {stream_packet_list_it_ = it;}

  void SetError() {error_ = 1;}
  int GetError() {return error_;}

  void ShowContent();
  void Show();


  // for Test.C
  void SetContentSize(u_int size);
};

#endif  // PACKET_H_
