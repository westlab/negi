/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: HttpDecoder.C,v 5.8 2011-11-18 04:36:20 sin Exp $
*/

#include "include.h"
#include "packet.h"
#include "stream.h"
#include "gzip.h"
#include "http_decoder.h"
#include "global.h"
#include "glog/logging.h"


HttpDecoder::HttpDecoder() {
    localbuf_ = reinterpret_cast<u_char *>(malloc(sizeof(SAVE_MAX_SIZE)));
    return;
}

HttpDecoder::~HttpDecoder() {
    free(localbuf_);
    return;
}

void HttpDecoder::Proc(Packet *packet) {
    if (packet->GetStream() == NULL) { return; }

    Stream *stream = packet->GetStream();

    LOG(INFO) << "Analyze";
    if (stream->GetState() == BEGIN) AnalyzeHeader(packet);
    LOG(INFO) << "Chunk";
    if (stream->GetHttpChunked()) DecodeChunk(packet);
    LOG(INFO) << "Gzip";
    if (!stream->GetL7Error()) {
        if (stream->GetHttpCompress()) DecodeGzip(packet);
    }
}

void HttpDecoder::AnalyzeHeader(Packet *packet) {
    Stream *stream = packet->GetStream();
    char* content = reinterpret_cast<char *>(packet->GetContent());

    // Is this stream for port 80?
    if (!(stream->GetDstPort() == 80 || stream->GetDstPort() == 8080 ||
          stream->GetSrcPort() == 80 || stream->GetSrcPort() == 8080)) {
        return;
    }

    // Search for 'HTTP/1.1'
    if (strncmp(content, "HTTP/1.1", sizeof("HTTP/1.1")-1) != 0 ||
        strncmp(content, "HTTP/1.0", sizeof("HTTP/1.0")-1) != 0 ) {
        // if not match, return
        stream->SetL7Protocol(HTTP);
    } else {
        // Stream is not HTTP
        return;
    }


    // Search for header end(CR LF CR LF)
    char *header_end = strstr(content, "\r\n\r\n");
    if (header_end ==NULL) {
        return;
    } else {
        stream->SetHttpHeaderSize((header_end - content) + sizeof("\r\n\r\n")-1);
        if (packet->GetContentSize() == stream->GetHttpHeaderSize()) {
            // there is HTTP header, but no content.
            if (stream->GetState() == BEGIN) {
                stream->SetGzipOnlyHttpHeader(1);
            } else {
                return;
            }
        }
    }

    // Search for Transfer-Encoding: chunked
    if (strstr(content, "Transfer-Encoding: chunked") != NULL) {
        stream->SetHttpChunked(1);
    }

    // Search for Content-Length
    char *res = strstr(content, "Content-Length: ");
    if (res != NULL) {
        res += sizeof("Content-Length: ")-1;
        stream->SetHttpContentSize(atol(res));
    }

    if (strstr(content, "Content-Encoding: gzip") != NULL) {
        stream->SetHttpCompress(GZIP);
    } else if (strstr(content, "Content-Encoding: DEFLATE") != NULL) {
        stream->SetHttpCompress(DEFLATE);
    }
}

void HttpDecoder::DecodeChunk(Packet *packet) {
    LOG(INFO) << "Chunk Start";
    Stream *stream = packet->GetStream();
    u_char* content = packet->GetContent();

    u_char *l7_content = reinterpret_cast<u_char*>(malloc(packet->GetContentSize()));
    // enough size for dechunk v4 packet.
    packet->SetL7Content(l7_content);

    u_char *p_l7_content_work;
    u_char *p_content_work;
    // u_int l7_content_size = packet->GetContentSize();
    u_int l7_content_size = 0;

    memcpy(l7_content, content, packet->GetContentSize());

    if (stream->GetState() == BEGIN) {
        p_content_work = content + stream->GetHttpHeaderSize();
        p_l7_content_work = l7_content + stream->GetHttpHeaderSize();
        l7_content_size = stream->GetHttpHeaderSize();
    // MSG(content);
    } else if (stream->GetState() == END) {
        return;
    } else {
        // large chank
        if (packet->GetContentSize() < stream->GetChunkOffset()) {
            stream->SetChunkOffset(stream->GetChunkOffset() - packet->GetContentSize());
            packet->SetL7ContentSize(packet->GetContentSize());
            return;
        }
        p_content_work = content + stream->GetChunkOffset();
    // GetChunkOffset is rest size of devided chank
        p_l7_content_work = l7_content + stream->GetChunkOffset();
        l7_content_size = stream->GetChunkOffset();
    }
    u_int chunk_size = 0;

    while (1) {
        LOG(INFO) << "Chunk loop";
        // get chunk size.
        u_char * first_nonnum;

        // 10 is  base of hexadecimal(0x) number.
        chunk_size = strtol((const char *)p_content_work,
                     reinterpret_cast<char **>(&first_nonnum), 16);

        p_content_work = first_nonnum + sizeof("\r\n")-1;
        if (p_content_work - content  >= packet->GetContentSize()) {
            stream->SetChunkOffset(chunk_size);
            return;
        }
        if (strstr(reinterpret_cast<char *>(first_nonnum), "\r\n")
        != reinterpret_cast<char*>(first_nonnum)) {
            p_content_work = reinterpret_cast<u_char*>(strstr(reinterpret_cast<char *>
                             (first_nonnum), "\r\n" + sizeof("\r\n")-1));
            if (p_content_work - first_nonnum > 10 || p_content_work - first_nonnum < 0) {
                return;
            }

            LOG(INFO) << "first_nonnum:"<< first_nonnum;
            LOG(INFO) << "content:"<< content;
            LOG(INFO) << "contentwork:"<< (p_content_work-content);
            LOG(INFO) << "contentwork:"<< (p_content_work-first_nonnum);
            LOG(INFO) << "contentwork:"<< static_cast<int>(p_content_work[0]);
            LOG(INFO) << "headersize:"<< stream->GetHttpHeaderSize();
        }

        // debug
        if (chunk_size == LONG_MAX || chunk_size == LONG_MIN) {
            // chunk size error!
            LOG(ERROR) << "error state: "<< stream->GetState();
            LOG(ERROR) << "error offset: "<< stream->GetChunkOffset();
            LOG(ERROR) << "error chunk size: "<< chunk_size;
            LOG(ERROR) << "error content size: "<< packet->GetContentSize();
            LOG(ERROR) << "error calc size: "<< first_nonnum - content;
            LOG(ERROR) << "error calc size: "<< p_content_work - content;
            LOG(ERROR) << content;
            LOG(ERROR) << first_nonnum;
            LOG(ERROR) << p_content_work;
            stream->SetL7Error(1);
            return;
        }

        if (chunk_size == 8) {
            LOG(ERROR) << "error content: " << content;
            LOG(ERROR) << "error p_content_work: " << p_content_work;
        }

        if (chunk_size == 0) {
            // message is end.
            stream->SetChunkOffset(0);
            packet->SetL7ContentSize(l7_content_size);
            return;

        } else if (p_content_work - content + chunk_size > packet->GetContentSize()) {
            // chunk size > packet end. save offset and return.
            u_int save_size = packet->GetContentSize() - (p_content_work - content);
            u_int offset_size = chunk_size - save_size;
            memcpy(p_l7_content_work, p_content_work, save_size);
            stream->SetChunkOffset(offset_size);
            packet->SetL7ContentSize(l7_content_size + save_size);
            return;

        } else {
            // Copy chunk and goto next chunk
            memcpy(p_l7_content_work, p_content_work, chunk_size);
            p_l7_content_work += chunk_size;
            l7_content_size += chunk_size;
            p_content_work += chunk_size + sizeof("\r\n")-1;
        }
    }
}

void HttpDecoder::DecodeGzip(Packet *packet) {
    Stream *stream = packet->GetStream();
    LOG(INFO) << "Gzip decoding start";

    u_char* p_dec_start = packet->GetContent();
    z_stream *z = stream->GetGzipZ();
    int insize = packet->GetContentSize();
    int outsize = 0;
    int offset = 0;

    if (packet->GetL7Content() != packet->GetContent()) {
        // second packet
        p_dec_start = packet->GetL7Content();
        insize = packet->GetL7ContentSize();
    } else {
        LOG(INFO) << "GetL7content and GetContent are same";
    }
    LOG(INFO) << "GetGzipOnlyHeepHeader";

    if (stream->GetGzipOnlyHttpHeader() == 1) {
        z = reinterpret_cast<z_stream*>(malloc(sizeof(z_stream)));
        stream->SetGzipZ(z);
        gzip_.dec_init(z);
    } else if (stream->GetGzipOnlyHttpHeader() == 2) {
        if (stream->GetHttpCompress() == GZIP) {
            LOG(INFO) << "only http header dec gzip-------------";
            offset = gzip_.dec_gzip(localbuf_, p_dec_start, insize, z);
        }
    } else if (stream->GetState() == BEGIN) {
        insize -= stream->GetHttpHeaderSize();
        p_dec_start += stream->GetHttpHeaderSize();
        z = reinterpret_cast<z_stream*>(malloc(sizeof(z_stream)));
        stream->SetGzipZ(z);
        gzip_.dec_init(z);

        if (stream->GetHttpCompress() == GZIP) {
            offset = gzip_.dec_gzip(localbuf_, p_dec_start, insize, z);
        } else if (stream->GetHttpCompress() == DEFLATE) {
            offset = gzip_.dec_zlib(localbuf_, p_dec_start, insize, z);
        }
        if (offset <= 0) {
            stream->SetL7Error(1);
            return;
        }
    } else {
    }

    // WARINIG!! localbuf is not used!!
    if (stream->GetGzipOnlyHttpHeader() != 1) {
        outsize = gzip_.dec_deflate(localbuf_, p_dec_start+offset, insize - offset, z);
    }
    // second packet after Gzip only HttpHeader
    if (stream->GetGzipOnlyHttpHeader() >= 1) {
        stream->IncGzipOnlyHttpHeader();
    }
    if (outsize <= 0) {
        // out size is under 0
        return;
    }
    u_char *content = packet->GetContent();

    u_char *old_l7content = NULL;
    if (packet->GetL7Content() != packet->GetContent()) {
        content = packet->GetL7Content();
        old_l7content = packet->GetL7Content();
    }

    u_char *l7content = NULL;
    if (stream->GetGzipOnlyHttpHeader() != 1) {
        if (stream->GetState() == BEGIN) {
            l7content = reinterpret_cast<u_char *>(malloc(outsize + stream->GetHttpHeaderSize()));
            memcpy(l7content, content , stream->GetHttpHeaderSize());
            memcpy(l7content + stream->GetHttpHeaderSize(), gzip_.outbuffer , outsize);
            packet->SetL7Content(l7content);
            packet->SetL7ContentSize(stream->GetHttpHeaderSize() + outsize);
        } else {
            l7content = reinterpret_cast<u_char *>(malloc(outsize + stream->GetHttpHeaderSize()));
            memcpy(l7content, gzip_.outbuffer, outsize);
            packet->SetL7Content(l7content);
            packet->SetL7ContentSize(outsize);
        }
    }

    LOG(INFO)<< packet->GetL7Content();
    if (old_l7content != NULL) {
        free(old_l7content);
    }
    return;
}
