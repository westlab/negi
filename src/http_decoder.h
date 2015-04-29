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

#ifndef HTTP_DECODER_H_
#define HTTP_DECODER_H_
#pragma once
#include "packet.h"
#include "gzip.h"

#define CHUNK_BUF_SIZE 2000

#ifdef HTTP_DEBUG_EN
#define HTTP_DEBUG(x)  x;
#else
#define HTTP_DEBUG(x);
#endif


class  HttpDecoder {
 public:
        HttpDecoder();
        ~HttpDecoder();
        void Proc(Packet *packet);
        void AnalyzeHeader(Packet *packet);
        void DecodeChunk(Packet *packet);
        void DecodeGzip(Packet *packet);

 private:
        int decode();
        char* chunk_buf_;       // temporary buffer for data process
        char* st_work_;         // private copied original token data
        char* p_st_work_;       // current pos. of st_work
        char* p_st_bin_;        // current pos. of token's "cooked" buffer
        unsigned int st_work_size_;   // current size of yet not decoded data
        char* msg_buf_;
        Gzip gzip_;
        u_char *localbuf_;
        unsigned int header_size_;
        HttpCompress msg_cmp_;
        int msg_chunked_;
        int msg_end_;
        int offset_;

        void HeaderCheck();
};
#endif  // HTTP_DECODER_H_
