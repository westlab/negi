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

#include "Include.H"
#include "Global.H"
#include "MatchPreFilter.H"
#include "PacketCapture.H"

unsigned long sim_time;

void pcap_status(int x){
	pcap_t *pd;
	pd = pcap_descriptor;

//	pcap_breakloop(pd);

	pcap_stat status;
	pcap_stats(pd, &status);

	cout << "PCAP Library Infomation----" << endl;
	cout << "Packet Recieved: " << status.ps_recv << endl;
	cout << "Packet Dropped: " << status.ps_drop << endl;
	cout << "Packet Dropp Rate: " << (double)status.ps_drop / (double)status.ps_recv * 100.0 << "%" << endl;
	cout << "Packet IfDropped: " << status.ps_ifdrop << endl;

//	kill(getppid(), SIGTERM);
//	observer.Show();
}

void packetcap(){
		signal(SIGUSR1, pcap_status);

		//settings for libpcap
		pcap_t *pd = NULL;

		//static const int snaplen = 64;
		static const int snaplen = 15000;	//default packet snap length:15000bytes(enough for full capture)
		static const int pflag = 0;
		static const int timeout = 1000;
		static const int iterate = -1;
		//static const int iterate = 1000000;
		char ebuf[PCAP_ERRBUF_SIZE];
		bpf_u_int32 localnet, netmask;
		struct bpf_program fcode;


	if(config->get("type") != "pcap" && config->get("type") != "ether"){ 
		exit(1);
	}

	if(config->get("type") == "ether"){
		cerr << "Entering Ether mode." << endl;

		if (config->get("device").size() == 0) {
			fprintf(stderr, "You muse specify network interface.\n");
			fprintf(stderr, "pcap-tinysample dump_interface\n");
			exit(1);
		}

		/* open network interface with on-line mode */
		if ((pd = pcap_open_live(config->get("device").c_str(), snaplen, !pflag, timeout, ebuf)) == NULL) {
			fprintf(stderr, "Can't open pcap deivce\n");
			exit(1);
		}
		pcap_descriptor = pd;

		/* get informations of network interface */
		if (pcap_lookupnet(config->get("device").c_str(), &localnet, &netmask, ebuf) < 0) {
			fprintf(stderr, "Can't get interface informartions\n");
			exit(1);
		}
	}else if(config->get("type") == "pcap"){
		pd = pcap_open_offline(config->get("filename").c_str(), ebuf);
	}


    /* setting and compiling packet filter */
    if (pcap_compile(pd, &fcode, const_cast<char *>(config->get("pcap_filter").c_str()), 1, netmask) < 0) {
		fprintf(stderr, "can't compile fileter\n");
		exit(1);
    }
    if (pcap_setfilter(pd, &fcode) < 0) {
		fprintf(stderr, "can't set filter\n");
		exit(1);
    }

    //Setting Other settings
//	pcap_set_timeout(pd,1000000);
//	pcap_set_buffer_size(pd,100000);


	//set global variables for callback function.
	sim_time = atol(config->get("sim_time").c_str());


    /* loop packet capture util picking iterate packets up from interface. */
    if (pcap_loop(pd, iterate, pcap_callback, NULL) < 0) {
		(void)fprintf(stderr, "pcap_loop: error occurred\n");
	exit(1);
    }

    /* close capture device */
    pcap_close(pd);

	// close message queue
//	while(packet_queue->GetQueueSize() != 0){
//		MSG("pcap_loop: waiting for packet_queue flushed...");
//		sleep(1);
//	}
	kill(getppid(), SIGTERM);

}

void pcap_callback(u_char *userdata, const struct pcap_pkthdr *h, const u_char *p) {
	static struct timeval start_time;

	//Calc. ongoing time from Start time.
	if(start_time.tv_sec == 0 && start_time.tv_usec == 0) start_time = h->ts;
/*
	//Time limietd Run setting.
	if((h->ts).tv_sec - start_time.tv_sec > (long)sim_time) {
		cerr << "Simulation time end." << endl;
		//exit(0);
		pthread_exit(NULL);
	}
*/
	PacketCnt *pcnt;

	pcnt = (PacketCnt *)malloc(sizeof(PacketCnt) + h->caplen);
	memcpy(&(pcnt->pcap_hdr), h, sizeof(struct pcap_pkthdr));
	memcpy(pcnt->pcap_pkt, p, h->caplen);
	
	Packet *pkt = new Packet (pcnt);
	master->Proc(pkt);


}

