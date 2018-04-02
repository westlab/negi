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
#include "thread_header.h"





char *thread_mmap_buf;
circular_buffer **cb_threads;


void *packet_handler_thread(void *pcap_cap_thread_control){ 
    int* thread_ID = (int* )pcap_cap_thread_control;
    printf("thread %d started\n", (*thread_ID));
    clock_t thread_begin_time;
    clock_t thread_end_time;
    double avg_time = 0;
    int loop_count = 0;
    while (1){
        int buf_empty;
        if(MEASURE_TIME) loop_count++; 
        if(MEASURE_TIME) thread_begin_time = clock();        
        cb_buffer_struct pull_data;
        pull_data = CB_pop(cb_threads[(*thread_ID)], &buf_empty);
        if(!buf_empty){
            //printf("packet pulled from thread %d\n",thread_ID);
            //Packet *pkt = new Packet (pcnt);
            Packet *pkt = new Packet ((PacketCnt *)pull_data.buffer);
            master->Proc(pkt);
        }
        else{
            //printf(".");
        }
        if(MEASURE_TIME) thread_end_time = clock();
        if(MEASURE_TIME) avg_time = (double)(avg_time*(loop_count-1) + ((double)(thread_end_time - thread_begin_time)/ CLOCKS_PER_SEC))/(double)loop_count;
        if(MEASURE_TIME && (loop_count%10000 == 0)) printf("avg thread_exec time : %f ",avg_time);
    }
}

int main(int argc, char *argv[]) {

    const char *thread_buf_mmap_file_name = "/home/shanaka/repos/worker_buf";
    
    int file_size;  
    int options = PROT_READ | PROT_WRITE;
    int mode = MAP_SHARED;
    
    file_size = (MAX_THREADS +1)* sizeof(circular_buffer);
    
    thread_mmap_buf = (char *)set_up_mmap(thread_buf_mmap_file_name, file_size, options, mode);
    //[no_of_buffers];
    cb_threads = (circular_buffer **)malloc(MAX_THREADS*sizeof(long));
    int i = 0;
    for (i = 0; i<MAX_THREADS; i++){
        cb_threads[i] = (circular_buffer *)(thread_mmap_buf + i*sizeof(circular_buffer));
        CB_init(cb_threads[i]);
    }

// signal(SIGINT, signal_handler);
    google::InitGoogleLogging("negi");
    LOG(INFO) << "start negi";
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " config_file" << endl;
        return 1;
    }
    // Read arguments
    string config_file = argv[1];

    // Create instances
    config = new Config;
    config->init(config_file);

    master = new Master;
    rule_pool = new RulePool;
    stream_rebuild = new StreamRebuild;
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
    int thread_ID = 0;
    int pcap_cap_thread_control = 0;
    pthread_t pcap_cap_thread[MAX_THREADS];
    for ( thread_ID = 0; thread_ID < MAX_THREADS; thread_ID++){
        int pcap_cap_thread_control = thread_ID;
        printf("thread %d will start\n", thread_ID);
        pthread_create( &pcap_cap_thread[thread_ID],NULL,packet_handler_thread, &pcap_cap_thread_control );
    }
    

    packetcap();

    return 0;
}
