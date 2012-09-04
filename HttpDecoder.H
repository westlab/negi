/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: HttpDecoder.H,v 5.5 2012-05-09 17:24:13 sin Exp $
*/

#pragma once
#include "Packet.H"
#include "Gzip.H"

#define CHUNK_BUF_SIZE 2000

#ifdef HTTP_DEBUG_EN
#define HTTP_DEBUG(x)  x ;
#else
#define HTTP_DEBUG(x)   ;
#endif


class HttpDecoder{
	public:

		HttpDecoder();
		~HttpDecoder();
		void Proc(Packet *packet);
		void AnalyzeHeader(Packet *packet);
		void DecodeChunk(Packet *packet);
		void DecodeGzip(Packet *packet);

	private:
		int decode();
		char* chunk_buf;		//temporary buffer for data process
		char* st_work;		//private copied original token data
		char* p_st_work;	//current pos. of st_work
		char* p_st_bin;		//current pos. of token's "cooked" buffer
		unsigned int st_work_size;	//current size of yet not decoded data
		char* msg_buf;
		Gzip gzip;
		u_char *localbuf;
		/*
		boost::regex reg_gzip;
		boost::regex reg_deflate;
		boost::regex reg_length;
		boost::regex reg_chunk;
		boost::regex reg_header;
		boost::cmatch res;
		*/
		unsigned int header_size;
		HttpCompress msg_cmp;
		int msg_chunked;
		int msg_end;
		int offset;

		void HeaderCheck();

};

