/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Stream.H,v 5.9 2012/08/23 04:04:18 sin Exp $
*/
#pragma once
#include "include.h"
#include "packet.h"
#include "gzip.h"
#include "rule.h"
#include "match_prefilter.h"
#include "tcp_conn.h"
class Packet;
class TcpConn;
class MatchPreFilterState;

enum Direction{
    NEW, C2S, S2C
};

// == class for string match for harashima ==
class PapaResult{
    private:
        Rule* p_rule_;
        u_int rule_id_;
        u_int pat_len_;
        int place_of_packet_;

        unsigned char	result_string_[RESULT_SIZE];
        unsigned int	result_size_;
        unsigned int	result_offset_;
        bool	finished_;

    public:
        PapaResult();
        void SetPRule(Rule* p){p_rule_ = p;}
        Rule* GetPRule(){return p_rule_;}

        void SetRuleId(u_int id){rule_id_ = id;}
        u_int GetRuleId(){return rule_id_;}

        void SetPatLen(u_int len){pat_len_ = len;}
        u_int GetPatLen(){return pat_len_;}

        void SetPlaceOfPacket(int place){place_of_packet_ = place;}
        int GetPlaceOfPacket(){return place_of_packet_;}

        void SetResultString(unsigned char* src, u_int offset, u_int size){memcpy(result_string_ + offset, src, size);};
        unsigned char *GetResultString(){return result_string_;};
        void SetResultSize(u_int size){result_size_ = size;};
        unsigned int GetResultSize(){return result_size_;};
        void SetResultOffset(u_int offset){result_offset_ = offset;};
        unsigned int GetResultOffset(){return result_offset_;};
        void SetFinished(bool flag){finished_ = flag;};
        bool GetFinished(){return finished_;};
};
// == class for string match for harashima end ==

class Stream
{
    private:
        TcpConn *tcp_conn_;						// pointer to tcp connection that stream belong to
        unsigned int stream_id_;					// (temporally used) stream id number
        struct timeval timestamp_;				// first stream packet reach time.
        struct in6_addr src_ip_, dst_ip_;			// Source IP, Destination IP
        u_int src_port_, dst_port_;				// Source Port, Destination Port
        Direction direction_;					// transport direction(S2C: server->client, C2S: client-> server)
        unsigned int last_seq_no_;				// last reached packet's sequence number(for packet drop detection)

        bool disorder_flag_;						// packet disorder flag(internally used only)
        bool truncated_;							// not used.
        string error_;							// error messages
        char src_ip_str_[INET6_ADDRSTRLEN];
        char dst_ip_str_[INET6_ADDRSTRLEN];
        State state_;							// status of TCP stream(START,CONTINUE,FINISHED)

        list<Packet*> packet_list_;				// list of *packet belong to this stream
        list<Rule*> rule_list_;					// list of *rules apply to  this stream
        unsigned int packet_num_;				// number of packet belong to this stream
        unsigned int retrieved_content_size_;	// sum bytes of captured packet payloads
        unsigned int l7_retrieved_content_size_;	// sum bytes of decoded Layer7 data
        unsigned int last_packet_content_size_;	// last arrived packet payload size

        L7Protocol l7_protocol_;					// Layer7 protocol(ex: HTTP,FTP)
        u_int http_header_size_;					// HTTP header size(ended with CRLFCRLF)
        bool http_chunked_;						// HTTP chunked flag
        u_int http_content_size_;				// HTTP content size(from HTTP header)
        HttpCompress http_compress_;				// HTTP contents compression type
        u_int chunk_offset_;						// HTTP chunk encoding's current offset
        u_int chunk_ignore_;						// HTTP chunk encoding's ignore bytes at next packet

        z_stream *gzip_z_;						// zlib's intermidiate state

        bool save_flag_;							// hit flag for layer7 search
        bool l7_error_;							// error flag

        list<Stream*>::iterator stream_pool_it_;	// used for gabage collect
        list<Stream*>::iterator tcp_conn_it_;	// used for gabage collect

        //engine specific values
        MatchPreFilterState *match_prefilter_state_;
        list<PapaResult*> papa_result_list_;

        //Observer
        size_t stream_packet_size_;

        //Gabage Collector
        bool gc_deleted_;
        struct timeval	last_updated_time_;

        int gzip_only_http_header_;		//Flag this stream is gzipped but first packet contains only httpdeader
        char buffer_[BUFF_SIZE];
    public:
        Stream(Packet *pkt);
        ~Stream();
        void AddRule(Rule *rule);
        void AddPacket(Packet *pkt);
        void RemovePacketIt(list<Packet *>::iterator it);
        list<Rule*>::iterator GetRuleFirstIt();
        list<Rule*>::iterator GetRuleLastIt();
        list<Rule*>::iterator RemoveRuleIt(list<Rule*>::iterator it);
        Rule* GetRule(list<Rule*>::iterator it);
        unsigned int GetPacketNum();
        list<Packet*>::iterator GetPacketFirstIt(){return packet_list_.begin();}
        list<Packet*>::iterator GetPacketLastIt(){return packet_list_.end();}
        unsigned int GetRetrievedContentSize(){return retrieved_content_size_;}
        unsigned int GetL7RetrievedContentSize(){return l7_retrieved_content_size_;}
        void SetL7RetrievedContentSize(u_int size){l7_retrieved_content_size_ = size;}
        struct timeval GetTimestamp(){ return timestamp_;}
        struct in6_addr GetSrcIP(){return src_ip_;}
        struct in6_addr GetDstIP(){return dst_ip_;}
        char* GetSrcIPStr(){return src_ip_str_;}
        char* GetDstIPStr(){return dst_ip_str_;}
        unsigned int GetSrcPort(){return src_port_;}
        unsigned int GetDstPort(){return dst_port_;}
        unsigned int GetLastSeqNo(){return last_seq_no_;}

        void SetDirection(Direction dir){direction_ = dir;}
        Direction GetDirection(){return direction_;}
        bool GetTruncated(){return truncated_;}
        unsigned int GetStreamSize(){return retrieved_content_size_;}
        unsigned int GetOrgStreamSize(){return 0;}
        void SetState(State st){state_ = st;}
        State GetState(){return state_;}
        string GetError(){return error_;}

        void SetL7Protocol(L7Protocol pt){l7_protocol_ =  pt;}
        L7Protocol GetL7Protocol(){return l7_protocol_;}
        void SetHttpContentSize(u_int size){http_content_size_ = size;}
        unsigned int GetHttpContentSize(){return http_content_size_;}
        void SetHttpCompress(HttpCompress cmp){http_compress_ = cmp;}
        HttpCompress GetHttpCompress(){return http_compress_;}
        void SetHttpHeaderSize(u_int size){http_header_size_ = size;}
        u_int GetHttpHeaderSize(){return http_header_size_;}
        void SetHttpChunked(bool ck){http_chunked_ = ck;}
        bool GetHttpChunked(){return http_chunked_;}
        void SetChunkOffset(u_int offset){chunk_offset_ = offset;}
        u_int GetChunkOffset(){return chunk_offset_;}
        unsigned int GetLastPacketContentSize(){return last_packet_content_size_;}

        void SetL7Error(bool er){l7_error_ = er;}
        bool GetL7Error(){return l7_error_;}

        int GetHitCount(){return 0;}
        void SetTcpConn(TcpConn * conn){tcp_conn_ = conn;}
        TcpConn* GetTcpConn(){return tcp_conn_;}
        string GetMatchString(){return "";}
        string GetRuleIds();
        void ShowStreamInfo();
        void ShowPackets();
        void ShowContents();
        void Show();
        void Reconstruct();

        list<Stream*>::iterator GetStreamPoolIt(){return stream_pool_it_;}
        void SetStreamPoolIt(list<Stream*>::iterator it){stream_pool_it_ = it;}
        list<Stream*>::iterator GetTcpConnIt(){return tcp_conn_it_;}
        void SetTcpConnIt(list<Stream*>::iterator it){tcp_conn_it_ = it;}
        unsigned long int GetStreamId(){return stream_id_;}
        //engine specific functions
        MatchPreFilterState *GetMatchPreFilterState();
        void SetMatchPreFilterState(MatchPreFilterState *state);

        void AddPapaResult(PapaResult *pres){papa_result_list_.push_back(pres);};
        list<PapaResult*>::iterator DeletePapaResultIt(list<PapaResult*>::iterator it){delete *it; return papa_result_list_.erase(it);};
        list<PapaResult*>::iterator GetPapaResultListFirstIt(){return papa_result_list_.begin();};
        list<PapaResult*>::iterator GetPapaResultListLastIt(){return papa_result_list_.end();};
        size_t GetPapaResultListSize(){return papa_result_list_.size();};

        //garbage collector
        struct timeval GetLastUpdatedTime(){ return last_updated_time_;};
        void GabageDelete();
        bool GetGabageState(){return gc_deleted_;};
        void SetSaveFlag(){save_flag_ = 1;};
        bool GetSaveFlag(){return save_flag_;};

        void SetGzipZ(z_stream *z){gzip_z_ = z ;};
        z_stream* GetGzipZ(){return gzip_z_ ;};

        void SetGzipOnlyHttpHeader(int _gzip_only_http_header){ gzip_only_http_header_ = _gzip_only_http_header;}
        int GetGzipOnlyHttpHeader(){return gzip_only_http_header_;}
        void IncGzipOnlyHttpHeader(){ gzip_only_http_header_++;}
};
