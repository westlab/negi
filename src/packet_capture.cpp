/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: PacketCapture.C,v 5.23 2012-07-04 03:41:21 sin Exp $
*/

#include "include.h"
#include "global.h"
#include "match_prefilter.h"
#include "packet_capture.h"
#include "glog/logging.h"



unsigned long sim_time;

void pcap_status(int x) {
    pcap_t *pd;
    pd = pcap_descriptor;

    pcap_stat status;
    pcap_stats(pd, &status);

    LOG(INFO) << "PCAP Library Infomation----";
    LOG(INFO) << "Packet Recieved: " << status.ps_recv;
    LOG(INFO) << "Packet Dropped: " << status.ps_drop;
    LOG(INFO) << "Packet Dropp Rate: " << static_cast<double>(status.ps_drop)
                 / static_cast<double>(status.ps_recv * 100.0) << "%";
    LOG(INFO) << "Packet IfDropped: " << status.ps_ifdrop;
}

void packetcap() {
        signal(SIGUSR1, pcap_status);

        // settings for libpcap
        pcap_t *pd = NULL;

        static const int snaplen = 15000;
        // default packet snap length:15000bytes(enough for full capture)
        static const int pflag = 0;
        static const int timeout = 1000;
        static const int iterate = -1;
        char ebuf[PCAP_ERRBUF_SIZE];
        bpf_u_int32 localnet, netmask;
        struct bpf_program fcode;

    if (config->get("type") != "pcap" && config->get("type") != "ether") {
        exit(1);
    }

    if (config->get("type") == "ether") {
        cerr << "Entering Ether mode." << endl;

        if (config->get("device").size() == 0) {
            fprintf(stderr, "You muse specify network interface.\n");
            fprintf(stderr, "pcap-tinysample dump_interface\n");
            exit(1);
        }

        /* open network interface with on-line mode */
        if ((pd = pcap_open_live(config->get("device").c_str(),
        snaplen, !pflag, timeout, ebuf)) == NULL) {
            fprintf(stderr, "Can't open pcap deivce\n");
            exit(1);
        }
        pcap_descriptor = pd;

        /* get informations of network interface */
        if (pcap_lookupnet(config->get("device").c_str(), &localnet, &netmask, ebuf) < 0) {
            fprintf(stderr, "Can't get interface informartions\n");
            exit(1);
        }
    } else if (config->get("type") == "pcap") {
        pd = pcap_open_offline(config->get("filename").c_str(), ebuf);
    }

    /* setting and compiling packet filter */
    if (pcap_compile(pd, &fcode, const_cast<char *>(config->get("pcap_filter").c_str()),
    1, netmask) < 0) {
        fprintf(stderr, "can't compile fileter\n");
        exit(1);
    }
    if (pcap_setfilter(pd, &fcode) < 0) {
        fprintf(stderr, "can't set filter\n");
        exit(1);
    }

    // Setting Other settings

    // set global variables for callback function.
    sim_time = atol(config->get("sim_time").c_str());

    /* loop packet capture util picking iterate packets up from interface. */
    if (pcap_loop(pd, iterate, pcap_callback, NULL) < 0) {
        (void)fprintf(stderr, "pcap_loop: error occurred\n");
    exit(1);
    }

    /* close capture device */
    pcap_close(pd);

    // close message queue
    kill(getppid(), SIGTERM);
}

void pcap_callback(u_char *userdata, const struct pcap_pkthdr *h, const u_char *p) {
    static struct timeval start_time;

    // Calc. ongoing time from Start time.
    if (start_time.tv_sec == 0 && start_time.tv_usec == 0) start_time = h->ts;
    PacketCnt *pcnt;

    pcnt = reinterpret_cast<PacketCnt *>(malloc(sizeof(PacketCnt) + h->caplen));
    memcpy(&(pcnt->pcap_hdr), h, sizeof(struct pcap_pkthdr));
    memcpy(pcnt->pcap_pkt, p, h->caplen);




    struct ether_header* eth_header_;      // pointer to Ether header structure
    struct vlan_ethhdr* vlan_eth_header_;  // pointer to VLAN tagged Ether header structure
    struct iphdr* ip_header_;              // pointer to IP header structure
    struct ip6_hdr* ip6_header_;           // pointer to IP header structure
    struct tcphdr* tcp_header_;            // pointer to TCP header structure
    unsigned int l2_header_size_;          // L2(MAC) header size
    unsigned int ether_proto_;
    bool vlan_tag_flag_;                  // TRUE if vlan tagged

    unsigned int l3_header_size_;         // L3(IP) header size
    unsigned int l4_header_size_;         // L4(TCP/UDP) header size
    unsigned int version_;                // Packet IP Version(4 or 6)

    unsigned char* l3_header_;            // pointer to L3(MAC) header
    unsigned char* l4_header_;            // pointer to L4(TCP/UDP) header

    unsigned int protocol_;               // Transport Protocol(ex: TCP:6) (in IPv6: Next Header)

    struct in6_addr src_ip_;              // Source IP(IPv4 included v6 with ::ffff:x.x.x.x)
    struct in6_addr dst_ip_;              // Destination IP

    char src_ip_str_[INET6_ADDRSTRLEN];
    char dst_ip_str_[INET6_ADDRSTRLEN];

    unsigned int src_port_;               // Source Port
    unsigned int dst_port_;               // Destination Port



    eth_header_ = (struct ether_header *) p;
    //memcpy(packet_, packet_cnt_->pcap_pkt, h->caplen);

    l2_header_size_ = sizeof(struct ether_header);
    eth_header_ = (struct ether_header *) p;

    if (ntohs(eth_header_->ether_type) == ETH_P_8021Q) {
        l3_header_ = (unsigned char*)(p) + sizeof(struct vlan_ethhdr);
        vlan_eth_header_ = (struct vlan_ethhdr *) p;
        ether_proto_ = ntohs(vlan_eth_header_->ether_type);
    } else {
        l3_header_ = (unsigned char *)(p) + sizeof(struct ether_header);  // IP header
        ether_proto_ = ntohs(eth_header_->ether_type);
    }

    switch (ether_proto_) {
        case ETH_P_IP:
            //LOG(ERROR) << "This is IPv4"; //shanaka
            version_ = 4;
            ip_header_ = (struct iphdr *)l3_header_;
            inet_v4tov6((struct in_addr *)(&(ip_header_->saddr)), &src_ip_);
            inet_v4tov6((struct in_addr *)(&(ip_header_->daddr)), &dst_ip_);
            protocol_ = ip_header_->protocol;

            l3_header_size_ = ip_header_->ihl*4;

            l4_header_ = l3_header_ + ip_header_->ihl*4;  // TCP/UDP header

            struct in_addr v4_src_ip, v4_dst_ip;
            inet_v6tov4(&src_ip_ , &v4_src_ip);
            inet_v6tov4(&dst_ip_ , &v4_dst_ip);

            inet_ntop(AF_INET, &v4_src_ip, src_ip_str_, INET6_ADDRSTRLEN);
            inet_ntop(AF_INET, &v4_dst_ip, dst_ip_str_, INET6_ADDRSTRLEN);

            ip6_header_ = NULL;

            break;

        case ETH_P_IPV6:
            //LOG(ERROR) << "This is IPv6"; //shanaka
            version_ = 6;
            ip6_header_ = (struct ip6_hdr *)l3_header_;
            src_ip_ = ip6_header_->ip6_src;
            dst_ip_ = ip6_header_->ip6_dst;
            protocol_ = ip6_header_->ip6_ctlun.ip6_un1.ip6_un1_nxt;

            l3_header_size_ = 40;  // FIXED Size. no follow about extension header.

            l4_header_ = l3_header_ + 40;  // TCP/UDP header
            // in IPv6, IP Header size is not included in payload length(ip6_un1_plen).

            inet_ntop(AF_INET6, &src_ip_, src_ip_str_, INET6_ADDRSTRLEN);
            inet_ntop(AF_INET6, &dst_ip_, dst_ip_str_, INET6_ADDRSTRLEN);

            ip_header_ = NULL;
            break;

        default:
            src_port_ = 0;
            dst_port_ = 0;
            return;
    }
    if (protocol_ == IPPROTO_TCP) {
        tcp_header_ = (struct tcphdr *)l4_header_;
        src_port_ = ntohs(tcp_header_->source);
        dst_port_ = ntohs(tcp_header_->dest);
    } else if (protocol_ == IPPROTO_UDP) {
        struct udphdr* udp_header = (struct udphdr *)l4_header_;
        src_port_ = ntohs(udp_header->source);
        dst_port_ = ntohs(udp_header->dest);
    } else {
        src_port_ = 0;
        dst_port_ = 0;
    }


    if(sizeof(PacketCnt) < 10000){
        int thread_ID = 0;
        thread_ID = (src_port_ | dst_port_)%MAX_THREADS;
        cb_buffer_struct push_data;
        push_data.rule_id = 1;
        //printf("packet pushed to thread %d\n",thread_ID);
        memcpy(push_data.buffer,pcnt, sizeof(PacketCnt));
        CB_push( cb_threads[thread_ID], push_data);
        
        /*
        cb_buffer_struct pull_data;
        int buf_empty;
        pull_data = CB_pop(cb_threads[thread_ID], &buf_empty);

        //Packet *pkt = new Packet (pcnt);
        Packet *pkt = new Packet ((PacketCnt *)pull_data.buffer);
        master->Proc(pkt);
        */
    }
    
}
