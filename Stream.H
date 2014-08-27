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
#include "Include.H"
#include "Packet.H"
#include "Gzip.H"
#include "Rule.H"
#include "MatchPreFilter.H"
#include "TcpConn.H"
class Packet;
class TcpConn;
class MatchPreFilterState;

enum Direction{
	NEW, C2S, S2C
};

// == class for string match for harashima ==
class PapaResult{
	private:
		Rule* p_rule;
		u_int rule_id;
		u_int pat_len;
		int place_of_packet;

		unsigned char	result_string[RESULT_SIZE];
		unsigned int	result_size;
		unsigned int	result_offset;
		bool	finished;

	public:
		PapaResult();
		void SetPRule(Rule* p){p_rule = p;}
		Rule* GetPRule(){return p_rule;}

		void SetRuleId(u_int id){rule_id = id;}
		u_int GetRuleId(){return rule_id;}

		void SetPatLen(u_int len){pat_len = len;}
		u_int GetPatLen(){return pat_len;}

		void SetPlaceOfPacket(int place){place_of_packet = place;}
		int GetPlaceOfPacket(){return place_of_packet;}

		void SetResultString(unsigned char* src, u_int offset, u_int size){memcpy(result_string + offset, src, size);};
		unsigned char *GetResultString(){return result_string;};
		void SetResultSize(u_int size){result_size = size;};
		unsigned int GetResultSize(){return result_size;};
		void SetResultOffset(u_int offset){result_offset = offset;};
		unsigned int GetResultOffset(){return result_offset;};
		void SetFinished(bool flag){finished = flag;};
		bool GetFinished(){return finished;};
};
// == class for string match for harashima end ==

class Stream
{
	private:
		TcpConn *tcp_conn;						// pointer to tcp connection that stream belong to
		unsigned int stream_id;					// (temporally used) stream id number
		struct timeval timestamp;				// first stream packet reach time.
		struct in6_addr src_ip, dst_ip;			// Source IP, Destination IP
		u_int src_port,dst_port;				// Source Port, Destination Port
		Direction direction;					// transport direction(S2C: server->client, C2S: client-> server)
		unsigned int last_seq_no;				// last reached packet's sequence number(for packet drop detection)

		bool disorder_flag;						// packet disorder flag(internally used only)
		bool truncated;							// not used.
		string error;							// error messages
		char src_ip_str[INET6_ADDRSTRLEN];
		char dst_ip_str[INET6_ADDRSTRLEN]; 		
		State state;							// status of TCP stream(START,CONTINUE,FINISHED)

		list<Packet*> packet_list;				// list of *packet belong to this stream
		list<Rule*> rule_list;					// list of *rules apply to  this stream
		unsigned int packet_num;				// number of packet belong to this stream
		unsigned int retrieved_content_size;	// sum bytes of captured packet payloads
		unsigned int l7_retrieved_content_size;	// sum bytes of decoded Layer7 data
		unsigned int last_packet_content_size;	// last arrived packet payload size
		
		L7Protocol l7_protocol;					// Layer7 protocol(ex: HTTP,FTP)
		u_int http_header_size;					// HTTP header size(ended with CRLFCRLF)
		bool http_chunked;						// HTTP chunked flag
		u_int http_content_size;				// HTTP content size(from HTTP header)
		HttpCompress http_compress;				// HTTP contents compression type
		u_int chunk_offset;						// HTTP chunk encoding's current offset
		u_int chunk_ignore;						// HTTP chunk encoding's ignore bytes at next packet

		z_stream *gzip_z;						// zlib's intermidiate state 

		bool save_flag;							// hit flag for layer7 search
		bool l7_error;							// error flag

		list<Stream*>::iterator stream_pool_it;	// used for gabage collect
		list<Stream*>::iterator tcp_conn_it;	// used for gabage collect

		//engine specific values
		MatchPreFilterState *match_prefilter_state;
		list<PapaResult*> papa_result_list;

		//Observer
		size_t stream_packet_size;

		//Gabage Collector
		bool gc_deleted;
		struct timeval	last_updated_time;

		int gzip_only_http_header;		//Flag this stream is gzipped but first packet contains only httpdeader
		char buffer[BUFF_SIZE];
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
		list<Packet*>::iterator GetPacketFirstIt(){return packet_list.begin();}
		list<Packet*>::iterator GetPacketLastIt(){return packet_list.end();}
		unsigned int GetRetrievedContentSize(){return retrieved_content_size;}
		unsigned int GetL7RetrievedContentSize(){return l7_retrieved_content_size;}
		void SetL7RetrievedContentSize(u_int size){l7_retrieved_content_size = size;}
		struct timeval GetTimestamp(){ return timestamp;}
		struct in6_addr GetSrcIP(){return src_ip;}
		struct in6_addr GetDstIP(){return dst_ip;}
		char* GetSrcIPStr(){return src_ip_str;}
		char* GetDstIPStr(){return dst_ip_str;}
		unsigned int GetSrcPort(){return src_port;}
		unsigned int GetDstPort(){return dst_port;}
		unsigned int GetLastSeqNo(){return last_seq_no;}

		void SetDirection(Direction dir){direction = dir;}
		Direction GetDirection(){return direction;}
		bool GetTruncated(){return truncated;}
		unsigned int GetStreamSize(){return retrieved_content_size;}
		unsigned int GetOrgStreamSize(){return 0;}
		void SetState(State st){state = st;}
		State GetState(){return state;}
		string GetError(){return error;}

		void SetL7Protocol(L7Protocol pt){l7_protocol =  pt;}
		L7Protocol GetL7Protocol(){return l7_protocol;}
		void SetHttpContentSize(u_int size){http_content_size = size;}
		unsigned int GetHttpContentSize(){return http_content_size;}
		void SetHttpCompress(HttpCompress cmp){http_compress = cmp;}
		HttpCompress GetHttpCompress(){return http_compress;}
		void SetHttpHeaderSize(u_int size){http_header_size = size;}
		u_int GetHttpHeaderSize(){return http_header_size;}
		void SetHttpChunked(bool ck){http_chunked = ck;}
		bool GetHttpChunked(){return http_chunked;}
		void SetChunkOffset(u_int offset){chunk_offset = offset;}
		u_int GetChunkOffset(){return chunk_offset;}
		unsigned int GetLastPacketContentSize(){return last_packet_content_size;}

		void SetL7Error(bool er){l7_error = er;}
		bool GetL7Error(){return l7_error;}

		int GetHitCount(){return 0;}
		void SetTcpConn(TcpConn * conn){tcp_conn = conn;}
		TcpConn* GetTcpConn(){return tcp_conn;}
		string GetMatchString(){return "";}
		string GetRuleIds();
		void ShowStreamInfo();
		void ShowPackets();
		void ShowContents();
		void Show();
		void Reconstruct();
		
		list<Stream*>::iterator GetStreamPoolIt(){return stream_pool_it;}
		void SetStreamPoolIt(list<Stream*>::iterator it){stream_pool_it = it;}
		list<Stream*>::iterator GetTcpConnIt(){return tcp_conn_it;}
		void SetTcpConnIt(list<Stream*>::iterator it){tcp_conn_it = it;}
		unsigned long int GetStreamId(){return stream_id;}
		//engine specific functions
		MatchPreFilterState *GetMatchPreFilterState();
		void SetMatchPreFilterState(MatchPreFilterState *state);

		void AddPapaResult(PapaResult *pres){papa_result_list.push_back(pres);};
		list<PapaResult*>::iterator DeletePapaResultIt(list<PapaResult*>::iterator it){delete *it; return papa_result_list.erase(it);};
//        void DeletePapaResultIt(list<PapaResult*>::iterator it){papa_result_list.erase(it); delete *it; };
		list<PapaResult*>::iterator GetPapaResultListFirstIt(){return papa_result_list.begin();};
		list<PapaResult*>::iterator GetPapaResultListLastIt(){return papa_result_list.end();};
		size_t GetPapaResultListSize(){return papa_result_list.size();};
//		void SetPPapaResultList(list<PapaResult*> *p_result_list);

		//garbage collector
		struct timeval GetLastUpdatedTime(){ return last_updated_time;};
		void GabageDelete();
		bool GetGabageState(){return gc_deleted;};
		void SetSaveFlag(){save_flag = 1;};
		bool GetSaveFlag(){return save_flag;};

		void SetGzipZ(z_stream *z){gzip_z = z ;};
		z_stream* GetGzipZ(){return gzip_z ;};

		void SetGzipOnlyHttpHeader(int _gzip_only_http_header){ gzip_only_http_header = _gzip_only_http_header;}
		int GetGzipOnlyHttpHeader(){return gzip_only_http_header;}
		void IncGzipOnlyHttpHeader(){ gzip_only_http_header++;}
};
