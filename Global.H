/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Global.H,v 5.15 2012-05-19 15:01:30 sin Exp $
*/

#pragma once

#include "Include.H"
#include "PacketPool.H"
#include "RulePool.H"
#include "StreamPool.H"
#include "StreamRebuild.H"
#include "PacketCapture.H"
#include "PacketFileReader.H"
#include "TcpConnPool.H"
#include "Config.H"
#include "IpFilter.H"
#include "HttpDecoder.H"
#include "Master.H"
#include "MatchPreFilter.H"
#include "Extractor.H"
#include "ResultSaver.H"
#include "ResultPool.H"
#include "Util.H"

#ifdef USE_POSTGRES
#include "PgsqlSaver.H"
#include "Pgsql.H"
#include "RuleLoaderPgsql.H"
#else
#include "RuleLoaderFile.H"
#endif
#include "SqliteDao.H"
#include "SqliteSaver.H"

#include "PacketClean.H"
#include "Observer.H"
#include "Global.H"
#include "FileWriter.H"
//#include "RiverGate.H"


extern unsigned int stream_buf_size;
extern unsigned int emergency_save_all;
extern unsigned int no_stream_save;
extern unsigned int no_stream_org_save;
extern unsigned int no_regexp_hit_save;
extern map<string,string> sql_config;

extern pid_t pid[P_MAX];

extern SaveMode savemode;
extern MallocMode mallocmode;


extern list<Stream *> end_stream_list;
extern pcap_t *pcap_descriptor;

extern time_t gc_remove_time;
extern unsigned int sim_rule_num;
extern unsigned int sim_rule_size;

extern unsigned int stream_id_prev;

extern unsigned long int max_stream_num;
extern unsigned long int max_stream_size;
extern unsigned long int max_sum_stream_size;

extern Config *config;
extern PacketPool *packet_pool;
extern StreamPool *stream_pool;
extern RulePool *rule_pool;
extern StreamRebuild *stream_rebuild;
extern IpFilter *ip_filter;
extern HttpDecoder *http_decoder;
extern PacketFileReader *packet_file_reader;
extern MatchPreFilter *match_pre_filter;
extern ResultPool *result_pool;
extern ResultSaver *result_saver;
extern Extractor *extractor;
#ifdef USE_POSTGRES
extern PgsqlSaver *pgsql_saver;
extern RuleLoaderPgsql *rule_loader_pgsql;
extern Pgsql *pgsql;
#else
extern RuleLoaderFile *rule_loader_file;
#endif
extern SqliteDao *sqlite_dao;
extern SqliteSaver *sqlite_saver;
extern TcpConnPool *tcp_conn_pool;
extern Master *master;
extern PacketClean *packet_clean;
extern Observer *observer;
extern Gzip *gzip;
extern FileWriter *file_writer;
//extern RiverGate *river_gate;

extern void packetcap();
extern void pcap_signal();

extern pthread_mutex_t sss_cond_mut;
extern pthread_cond_t sss_cond;
extern Packet* sss_send_value;

extern void inet_v4tov6(struct in_addr *v4 ,struct in6_addr *v6);
extern void inet_v6tov4(struct in6_addr *v6 ,struct in_addr *v4);

