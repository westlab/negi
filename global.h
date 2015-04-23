/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Global.h,v 5.15 2012-05-19 15:01:30 sin Exp $
*/

#pragma once

#include "include.h"
#include "packet_pool.h"
#include "rule_pool.h"
#include "stream_pool.h"
#include "stream_rebuild.h"
#include "packet_capture.h"
#include "packet_file_reader.h"
#include "tcp_conn_pool.h"
#include "config.h"
#include "ip_filter.h"
#include "http_decoder.h"
#include "master.h"
#include "match_prefilter.h"
#include "extractor.h"
#include "result_saver.h"
#include "result_pool.h"
#include "util.h"

#ifdef USE_POSTGRES
#include "PgsqlSaver.h"
#include "Pgsql.h"
#include "RuleLoaderPgsql.h"
#else
#include "rule_loader_file.h"
#endif
#include "sqlite_dao.h"
#include "sqlite_saver.h"
#include "gzip.h"
#include "packet_clean.h"
#include "observer.h"
#include "global.h"
#include "file_writer.h"
//#include "RiverGate.h"


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
extern RuleLoaderFile *rule_loader_file;
extern SqliteDao *sqlite_dao;
extern SqliteSaver *sqlite_saver;
extern TcpConnPool *tcp_conn_pool;
extern Master *master;
extern PacketClean *packet_clean;
extern Observer *observer;
extern Gzip *gzip;
extern FileWriter *file_writer;

extern void packetcap();
extern void pcap_signal();

extern void inet_v4tov6(struct in_addr *v4 ,struct in6_addr *v6);
extern void inet_v6tov4(struct in6_addr *v6 ,struct in_addr *v4);
