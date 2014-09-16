/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Proc.C,v 5.4 2012-05-09 17:24:13 sin Exp $
*/
#include "Proc.H"
#include "Global.H"

Proc::Proc(){
	return;
}

Proc::~Proc(){
	return;
}

void Proc::Init(){
}

void Proc::InitManager(){
	// lock shem
	// initialize shem
	// read config
	// unlock shem
	config = new Config;
	packet_pool = new PacketPool;
	rule_pool = new RulePool;
	stream_rebuild = new StreamRebuild;
	ip_filter = new IpFilter;
	match_pre_filter = new MatchPreFilter;
	extractor = new Extractor;
	stream_pool = new StreamPool;
#ifdef USE_POSTGRES
	pgsql = new Pgsql;
	pgsql_saver = new PgsqlSaver;
	rule_loader_pgsql = new RuleLoaderPgsql;
#endif
	tcp_conn_pool = new TcpConnPool;
	packet_clean = new PacketClean;
	observer = new Observer;
//	packet_queue = new PacketQueue;
	//river_gate = new RiverGate;

}

void Proc::InitPacket(){
	//wait shem unlock
}

void Proc::InitMaster(){
	//wait shem unlock
}




