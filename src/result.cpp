/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Result.C,v 5.0 2010-10-13 08:47:16 sin Exp $
*/
#include "result.h"
#include "global.h"

Result::Result(Stream *pstream,
               unsigned int fid,
               unsigned long int sof_size,
               unsigned int poffset_size,
               unsigned int pcontent_size,
               unsigned char * r_string){
    parent_stream_ = pstream;
    filter_id_ = fid;
    stream_offset_size_ = sof_size;
    packet_offset_size_ = poffset_size;
    packet_content_size_ = pcontent_size;
    memcpy(r_string, result_string_, RESULT_SIZE);

    return;
}

Result::~Result(){
    return;
}
