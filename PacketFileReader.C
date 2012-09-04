/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: PacketFileReader.C,v 5.3 2012-03-21 14:56:25 sin Exp $
*/

#include "Include.H"
#include "Global.H"
#include "MatchPreFilter.H"
#include "PacketFileReader.H"

PacketFileReader::PacketFileReader(){
	return;
}

void PacketFileReader::Init(){

	ifstream is;
	is.open(config->get("filename").c_str());
	if(!is){
		cout << "PacketFileReader: Can't open " << config->get("filename") << "."<< endl;
		exit(1);
	}
	while(!is.eof()){
		string timestamp, content_size, src_ip, src_port, dst_ip, dst_port, flag, content;

		is >> timestamp >> content_size >> src_ip >> src_port >> dst_ip >> dst_port >> flag >> content;
		if(content_size == "0"){
			observer->Show();
			exit(0);
			
			}

		Packet *pkt = new Packet( timestamp, content_size, src_ip, src_port, dst_ip, dst_port, flag, content);
		master->Proc(pkt);
	}
	
	exit(0);


}

