/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Master.C,v 5.41 2012-08-22 08:42:52 sin Exp $
*/
#include "master.h"
#include "global.h"
#include "glog/logging.h"

Master::Master(){
    observer_packet_counter = 0;
    all_packet_counter = 0;

    max_stream_size = 0;
    start_time = time(NULL);

    return;
}

Master::~Master(){
    return;
}
void Master::Proc(Packet *pkt){
    LOG(INFO) << "Master Proc";
    stream_rebuild->Proc(pkt);

    if(pkt->GetError() == 0){
        LOG(INFO) << "Http Decode";
        http_decoder->Proc(pkt);

        LOG(INFO) << "Match Prefilter";
        match_pre_filter->Proc(pkt);

        LOG(INFO) << "Extract";
        extractor->Proc(pkt);
    }

    // OBSERVER_DEBUG(observer->ShowMem(pkt->GetTimestamp()););

    static time_t prev_time;
    static time_t now_time;
    static unsigned int time_counter;

    if(!end_stream_list.empty()){
        for(list<Stream *>::iterator it=end_stream_list.begin(); it != end_stream_list.end(); it++){
            if((*it)->GetSaveFlag()){
                sqlite_saver->Proc(*it);
            }else if(atoi(config->get("save_all").c_str())){
                sqlite_saver->Proc(*it);
            }
        }
    }
    packet_clean->Proc(pkt);
}
