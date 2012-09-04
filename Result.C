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
#include "Result.H"
#include "Global.H"

Result::Result(Stream *pstream, unsigned int fid, unsigned long int sof_size, unsigned int poffset_size, unsigned int pcontent_size, unsigned char * r_string){
		parent_stream = pstream;
		filter_id = fid;
		stream_offset_size = sof_size;
		packet_offset_size = poffset_size;
		packet_content_size = pcontent_size;
		memcpy(r_string, result_string, RESULT_SIZE);

	return;
}

Result::~Result(){
	return;
}



