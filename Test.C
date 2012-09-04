/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 *$Id: Test.C,v 5.0 2010-10-13 08:47:17 sin Exp $
 */
#include <string>
#include "Include.H"
#include "Global.H"
#include "Test.H"
/*
Test::Test(){

	BLUE cout << "Initialization-----------------------"<<endl; RESET

	//Create Dummy Rules
//    rule_pool.AddRule(0, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "abcde", 0,100);
//    rule_pool.AddRule(1, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "efghij", 0,100);
//    rule_pool.AddRule(2, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "jkl", 0,20);
//    rule_pool.AddRule(3, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "anpanman", 0,20);
//    rule_pool.AddRule(4, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "gcagagag", 0,20);
//    rule_pool.AddRule(5, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "brown", 0,100);
//    rule_pool.AddRule(6, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "quick", 0,100);
//    rule_pool.AddRule(6, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "jumps", 0,100);
//    rule_pool.AddRule(6, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "fghijklmn", 0,100);
//    rule_pool.AddRule(6, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "BCDEFGHIJKLMNOPQRSTUVQZYZaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 0,10000);
//    rule_pool.AddRule(6, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "bcdefghijklmnopqrstuvqzyzAaaaaaaaaaaaaaaaaaaaaaaaaaa", 0,10000);//a 26
//    rule_pool.AddRule(6, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "baaaaaaaaaaaaaaaaaa", 0,800);
//    rule_pool.AddRule(6, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "westlab", 0,800);
	rule_pool.AddRule(6, "owner", "2009-12-09 15:00:12.12345", "192.168.1.2", "255.255.255.0", 1234, "192.168.1.1", "255.255.255.0", 80, "hoge", "abcdeeee", 0,800);
	//Show Rules
	rule_pool.ShowRules();

	//Create Dummy Packets
//    u_char pkt_str[50] = "abcdefghijklmnopqrstuvwx";
//    u_char pkt_str[200] = "A quick brown fox jumps over a lazy dog. abcdefghijklmnopqrstuvwx12345555";
//    u_char pkt_str[1000] = "abcAzzzzabcBzzzzabcCzzzzabcAzzzzabcBzzzzabcCzzzzabcAzzzzabcBzzzzabcCzzzzabcAzzzzabcBzzzzabcCzzzzabcAzzzzabcBzzzzabcCzzzzabcAzzzzabcBzzzzabcCzzzzabcAzzzzabcBzzzzabcCzzzzabcAzzzzabcBzzzzabcCzzzzabcAzzzzabcBzzzzabcCzzzz";
//	u_char pkt_str[10000] = "bcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
//    u_char pkt_str[10000] = "bcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdefghijklmnopqrstuvqzyzaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";//10 times, a52 other25
	u_char pkt_str[1000] = "caaaaaaaaaaaaaaaaaaccaaaaaaaaaaaaaaaaaaccaaaaaaaaaaaaaaaaaaccaaaaaaaaaaaaaaaaaaccaaaaaaaaaaaaaaaaaaccaaaaaaaaaaaaaaaaaaccaaaaaaaaaaaaaaaaaaccaaaaaaaaaaaaaaaaaaccaaaaaaaaaaaaaaaaaaccaaaaaaaaaaaaaaaaaac";
//    u_char pkt_str[1000] = "caaaaaaccccccaaaaaaccccccaaaaaaccccccaaaaaaccccccaaaaaaccccccaaaaaaccccccaaaaaaccccccaaaaaaccccccaaaaaaccccccaaaaaac";
	int pkt_size = 300;
	u_char *packet = (u_char *)malloc(sizeof(u_char)*1500);


	//creaet 6 packets
//    for(u_char *temp = pkt_str; *(temp + pkt_size - 1) != (u_char)NULL ; temp+= pkt_size){
	for(u_char *temp = pkt_str; *temp != (u_char)NULL ; temp+= pkt_size){
		//set timestamp to now.
		struct timeval time;
		gettimeofday(&time,NULL);

		//create packet
		u_int size = CreateDummyPacket(packet, "192.168.1.2","192.168.1.1",1234,80,temp,pkt_size);
		new Packet(packet, size, size, time);
	}

	//change content_size to 3
	for(list<Packet*>::iterator it = packet_pool.GetPacketFirstIt(); it != packet_pool.GetPacketLastIt(); it++){
		(*it)->SetContentSize(pkt_size);
	}

	//Show Packets
	RED cout << "Packet List-----------------------"<<endl; RESET
	packet_pool.ShowPackets();
	
	RED cout << "Execution-----------------------"<<endl; RESET
	
	//Create one test stream.
    Stream * test_stream = new Stream( *packet_pool.GetPacketFirstIt());
	
	//Add this stream to stream_pool.
//    stream_pool.AddStream(test_stream);
	Stream * test_stream = stream_pool.AddStream(*packet_pool.GetPacketFirstIt());

match_pre_filter.Proc(*(packet_pool.GetPacketFirstIt()));
//master.Proc(*(packet_pool.GetPacketFirstIt()));
	//Add continueing packets to stream.
	for(list<Packet*>::iterator it = ++packet_pool.GetPacketFirstIt(); it != packet_pool.GetPacketLastIt(); it++){
		test_stream->AddPacket(*it);
		match_pre_filter.Proc(*it);
//        master.Proc(*it);
	}


	//Process this stream!!
	for(list<Packet*>::iterator it = packet_pool.GetPacketFirstIt(); it != packet_pool.GetPacketLastIt(); it++){
		master.Proc(*it);
	}


	RED cout << "Results-----------------------"<<endl; RESET
	RED cout << "Stream List-----------------------"<<endl; RESET
	stream_pool.ShowStreams();

	return;

}

u_int Test::CreateDummyPacket(u_char *packet, string srcip, string dstip, u_int srcport, u_int dstport, u_char* cont, u_int cont_size){
	//Temporary create packet area. CALL DeleteDummyPacket!!

	//set src,dst address
	struct in_addr src_ip;
	struct in_addr dst_ip;
	inet_aton(srcip.c_str(),&src_ip);
	inet_aton(dstip.c_str(),&dst_ip);

	//Create IP Header
	u_char *ip = packet + sizeof(struct ether_header);
	struct ip* ip_header = (struct ip*)ip;
	ip_header->ip_src = src_ip;
	ip_header->ip_dst = dst_ip;
	ip_header->ip_p = IPPROTO_TCP;
	ip_header->ip_hl = 5; //Magic number for no option ip header

	//Create TCP header
	u_char* tcp = ip + ip_header->ip_hl*4;
	struct tcphdr* tcp_header = (struct tcphdr *)tcp;
	tcp_header->source = htons(srcport);
	tcp_header->dest = htons(dstport);
	tcp_header->doff = 5; //Magic number for no option tcp header

	//Create content
	u_char* content = tcp + tcp_header->doff*4;
	memcpy(content, cont, cont_size);
	
	return cont_size + (content - packet);
}
*/
