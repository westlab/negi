#include "include.h"
#include "global.h"
#include "stream.h"

#ifdef USE_SQLITE

SqliteSaver::SqliteSaver(){
    return;
}


void SqliteSaver::Proc(Stream * stream){
    //add
    struct timeval tmp_time = stream->GetTimestamp();
    struct tm *tmp = localtime(&tmp_time.tv_sec);
    ostringstream oss;
    oss << tmp->tm_year+1900 <<"-"<< tmp->tm_mon+1 <<"-"<<tmp->tm_mday <<" "<<tmp->tm_hour<<":"<<tmp->tm_min<<":"<<tmp->tm_sec;
    string tstamp = oss.str();


    oss.str("");
    oss << "insert into save_stream ("
    <<"id, srim_version, src_ip, dst_ip, src_port, dst_port, timestamp, "
    <<"direction ,truncate , regexp_id, "
    <<"st_size, st_size_org, "
    <<"error, "
    <<"l7_error, "
    <<"l7prot, http_size, http_compress, "
    <<"http_header_size, http_chunked, hit_count, "

    <<"match_str, stream, stream_org, "
    <<"after_ipfilter, after_prefilter, prefilter_log"
    <<") values ('"
    << stream->GetStreamId() << "','2.2','"<<stream->GetSrcIPStr()<<"','"<<stream->GetDstIPStr()<<"','"<<stream->GetSrcPort()<<"','"<<stream->GetDstPort()<<"','"<<tstamp<<"','"
    << stream->GetDirection() <<"','"<< stream->GetTruncated() <<"','"<< stream->GetRuleIds() <<"','"
//	<< stream->GetDirection() <<"','"<< stream->GetTruncated() <<"','""','"
    << stream->GetStreamSize() <<"','"<<  stream->GetOrgStreamSize() <<"','"
    << stream->GetError() <<"','"
    << stream->GetL7Error() <<"','"
    << stream->GetL7Protocol() <<"','"<<  stream->GetHttpContentSize() <<"','"<<  stream->GetHttpCompress() <<"','" \
    << stream->GetHttpHeaderSize() <<"','"<<  stream->GetHttpChunked() <<"','"<<  stream->GetHitCount();
    string query = oss.str();


    //match_str
    query += "','"+stream->GetMatchString()+"'";
    //stream
    if(!no_stream_save){
        query += ",'";
        query += "'";
    }else{
        query += ",''";
    }
    //stream_org
    if(!no_stream_org_save){
        query += ",''";
    }else{
        query += ",''";
    }

    //MatchPreFilter
    //after_ipfilter, after_prefilter, prefilter_log
    if(stream->GetMatchPreFilterState() != 0){
        oss.str("");
        oss << ", " << stream->GetMatchPreFilterState()->GetAfterIpFilter() << ", " << stream->GetMatchPreFilterState()->GetAfterPreFilter();
        query += oss.str() + ", '" + stream->GetMatchPreFilterState()->GetMatchPreFilterLog() + "'";
    }else{
        query += ", -1, -1, ''";
    }

    query += ");";
    cout << "save stream" << endl;
    cout << query << endl;

#ifdef FILEWRITE_MODE
        file_writer->Write(query);
#endif
#ifdef SQLITE_MODE
    sqlite_dao->ExecSql(query);
#endif
    oss.str("");

    return;
}


void SqliteSaver::ProcPacket(Packet * pkt){
    ostringstream oss;

    if(pkt->GetProtocol() != IPPROTO_TCP){return;}
    char ctstamp[100];
    struct timeval tmp_time = pkt->GetTimestamp();
    strftime(ctstamp, 100, "%Y-%m-%d %H:%M:%S", (const struct tm *)localtime(&tmp_time.tv_sec));
    string tstamp = ctstamp;
    /*
    oss << tmp->tm_year+1900 <<"-"<< tmp->tm_mon+1 <<"-"<<tmp->tm_mday <<" "<<tmp->tm_hour<<":"<<tmp->tm_min<<":"<<tmp->tm_sec;
    string tstamp = oss.str();
    */


    oss.str("");
    oss << "insert into save_packet (src_ip ,dst_ip ,src_port ,dst_port ,timestamp , \
    protocol, packet_size, packet_size_org, content_size, flag, content) values (\
    ,'"<< pkt->GetSrcIPStr() <<"','"<< pkt->GetDstIPStr() <<"','"<< pkt->GetSrcPort() <<"','"\
    <<pkt->GetDstPort() <<"','"<< tstamp <<"','"<<pkt->GetProtocol()<<"','"<< pkt->GetPacketSize() <<"','"<< pkt->GetPacketSizeOrg() <<"','"\
    << pkt->GetContentSize();
    string query = oss.str();

    //flag row
    query += "','";
    if(pkt->GetProtocol() == IPPROTO_TCP){
        if(pkt->GetAck()){ query += "[ACK]"; }
        if(pkt->GetFin()){ query += "[FIN]"; }
        if(pkt->GetSyn()){ query += "[SYN]"; }
        if(pkt->GetRst()){ query += "[RST]"; }
    }

    //query += "',E'"+T.esc_raw(pkt->GetContent(), pkt->GetContentSize())+"');";
    //query += "',E'"+escape_binary(pkt->GetContent(), pkt->GetContentSize())+"');";
    string content((char*) pkt->GetContent());
    query += "','"+content+"');";
    cout << "save packet" << endl;
    cout << query << endl;

    sqlite_dao->ExecBatchSql(query);
    oss.str("");

    return;
}

#endif //USE_SQLITE
