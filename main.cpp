/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 *$Id: srim.C,v 5.27 2012-08-22 08:05:53 sin Exp $
 */
#include "main.h"
#include "packet_capture.h"
#include "packet_file_reader.h"
#include "observer.h"

#include <functional>
#include <utility>
#include "glog/logging.h"


int main(int argc, char *argv[]){
//	signal(SIGINT, signal_handler);
    google::InitGoogleLogging("negi");
    LOG(INFO) << "start negi";
if(argc != 2){
    cerr << "Usage: " << argv[0] << " config_file" << endl;
    return 1;
}
    //Read arguments
    string config_file = argv[1];

    //Create instances
    config = new Config;
    config->init(config_file);

    master = new Master;
    rule_pool = new RulePool;
    stream_rebuild = new StreamRebuild;
    ip_filter = new IpFilter;
    http_decoder = new HttpDecoder;
    extractor = new Extractor;
    stream_pool = new StreamPool;
    tcp_conn_pool = new TcpConnPool;
    packet_clean = new PacketClean;
    observer = new Observer;
    gzip = new Gzip;
    file_writer = new FileWriter;
    rule_loader_file = new RuleLoaderFile;
    rule_loader_file->Proc();
    sqlite_dao = new SqliteDao;
    sqlite_dao->Connect(config->get("dbname"));
    sqlite_dao->CreateTableFromFile(config->get("sql_table"));
    sqlite_saver = new SqliteSaver;

    match_pre_filter = new MatchPreFilter;
    rule_pool->ShowRules();

    packetcap();

    return 0;
}
