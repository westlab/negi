/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Global.C,v 5.15 2012-05-19 15:01:30 sin Exp $
*/

#include "src/global.h"
#include <list>

unsigned int stream_buf_size;
unsigned int emergency_save_all;
unsigned int no_stream_save;
unsigned int no_stream_org_save;
unsigned int no_regexp_hit_save;

pid_t pid[P_MAX];

time_t gc_remove_time;
time_t now_time;

unsigned int sim_rule_num;
unsigned int sim_rule_size;
unsigned int stream_id_prev;

unsigned __int64 max_stream_num;
unsigned __int64 max_stream_size;
unsigned __int64 max_sum_stream_size;


list<Stream *> end_stream_list;

pcap_t *pcap_descriptor;

SaveMode savemode;
MallocMode mallocmode;

Config *config;
RulePool *rule_pool;
StreamRebuild *stream_rebuild;
HttpDecoder *http_decoder;
MatchPreFilter *match_pre_filter;
Extractor *extractor;
StreamPool *stream_pool;
RuleLoaderFile *rule_loader_file;
SqliteDao *sqlite_dao;
SqliteSaver *sqlite_saver;
Master *master;
TcpConnPool *tcp_conn_pool;
PacketClean *packet_clean;
Observer *observer;
Gzip *gzip;
FileWriter *file_writer;


void inet_v4tov6(struct in_addr *v4 , struct in6_addr *v6) {
    unsigned char v6char[] =
                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 0, 0, 0, 0};
    memcpy(v6char+12, (unsigned char *)v4, 4);
    memcpy(v6, v6char, 16);
}

void inet_v6tov4(struct in6_addr *v6 , struct in_addr *v4) {
    memcpy(v4, (unsigned char*)v6+12, 4);
}
