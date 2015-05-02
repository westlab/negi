/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Result.H,v 5.0 2010-10-13 08:47:16 sin Exp $
*/

#ifndef RESULT_H_
#define RESULT_H_

#pragma once
#include "packet.h"
#include "include.h"

class Result {
 private:
        Stream *parent_stream_;
        unsigned int filter_id_;
        unsigned long int stream_offset_size_;
        unsigned int packet_offset_size_;
        unsigned int packet_content_size_;
        unsigned char result_string_[RESULT_SIZE];
        unsigned int result_size_;
        bool finished;

 public:
        Result(Stream *pstream, unsigned int fid, unsigned long int sof_size,
               unsigned int poffset_size, unsigned int pcontent_size, unsigned char* r_string);
        ~Result();
        Stream *GetParentStream() {return parent_stream_; }
        unsigned int GetFilterId() {return filter_id_; }
        unsigned long int GetStreamOffsetSize() {return stream_offset_size_; }
        unsigned int GetPacketOffsetSize() {return packet_offset_size_; }
        unsigned int GetPacketContentSize() {return packet_content_size_; }
        unsigned char *GetResultString() {return result_string_; }
        unsigned int GetResultSize() {return result_size_; }
        bool GetFinished();
};

#endif  // RESULT_H_
