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
#pragma once
#include "Packet.H"
#include "Include.H"

class Result
{
	private:
		Stream *parent_stream;
		unsigned int	filter_id;
		unsigned long int	stream_offset_size;
		unsigned int	packet_offset_size;
		unsigned int	packet_content_size;
		unsigned char	result_string[RESULT_SIZE];
		unsigned int	result_size;
		bool	finished;

	public:
		Result(Stream *pstream, unsigned int fid, unsigned long int sof_size, unsigned int poffset_size, unsigned int pcontent_size, unsigned char* r_string);
		~Result();
		Stream *GetParentStream(){return parent_stream;};
		unsigned int GetFilterId(){return filter_id;};
		unsigned long int GetStreamOffsetSize(){return stream_offset_size;};
		unsigned int GetPacketOffsetSize(){return packet_offset_size;};
		unsigned int GetPacketContentSize(){return packet_content_size;};
		unsigned char *GetResultString(){return result_string;};
		unsigned int GetResultSize(){return result_size;};
		bool GetFinished();
};
