/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Gzip.C,v 5.7 2011-11-18 04:36:19 sin Exp $
 * Note: This is small inputbuffer  stream test!
*/
//Here is bug
////giwa
#include "include.h"
#include "gzip.h"
#include "global.h"


Gzip::Gzip(){

}
Gzip::~Gzip(){
}

int Gzip::dec_init(z_stream *z){
    //int status;

    // initialize zlib
    z->zalloc = Z_NULL;
    z->zfree = Z_NULL;
    z->opaque = Z_NULL;
    z->avail_in = 0;

    z->next_in = Z_NULL;
    if (inflateInit2(&(*z), -MAX_WBITS) != Z_OK){
        return -1;
    }

    return 0;
}

int Gzip::dec_gzip(u_char* outbuf, u_char* inbuf, int len, z_stream *z){

    if( inbuf[0] != (u_char)GZ_MAGIC_0 || inbuf[1] != (u_char)GZ_MAGIC_1 ){
        return -1;
    }
    int i = 2;	//byte count from first gzip
    int method = inbuf[i++];
    int flags  = inbuf[i++];
    if (method != Z_DEFLATED || (flags & RESERVED) != 0) {
        return -1;
    }
    i+=6;

    if (flags & EXTRA_FIELD) { // skip the extra field
        int len = 0;
        len  =  (uInt)inbuf[i++];
        len += ((uInt)inbuf[i++])<<8;
        i+=len;
    }
    if (flags & ORIG_NAME) { // skip the original file name
        while (inbuf[i++] !=0) ;
    }
    if (flags & COMMENT) {   // skip the .gz file comment
        while (inbuf[i++] != 0) ;
    }
    if (flags & HEAD_CRC) {  // skip the header crc
        i+=2;
    }
    return i;
}

int Gzip::dec_zlib(u_char* outbuf, u_char* inbuf, int len, z_stream *z){
    //zlib header is 2byte constant.
    return 2;
}


int Gzip::dec_deflate(u_char* outbuf, u_char* inbuf, int size, z_stream *z){
    //Setting Input buf data
    z->next_in = (Bytef*) inbuf;
    z->next_out = (Bytef*) outbuffer;
    z->avail_in = size;
    z->avail_out = SAVE_MAX_SIZE;

    int out_size = 0;
    int status;

    status = inflate(&*z, Z_SYNC_FLUSH);
    if (status != Z_STREAM_END && status != Z_OK) {
        return -1;
    }

    out_size = SAVE_MAX_SIZE - z->avail_out;
    return out_size;

}
