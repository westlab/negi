/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: StreamPool.H,v 5.1 2011-01-17 10:36:40 sin Exp $
*/

#pragma once

#include "Include.H"
#include "Stream.H"
#include "StreamPool.H"

class StreamPool
{
	private:
		list<Stream*> stream_pool;

	public:
		StreamPool();

        list<Stream*>::iterator AddStream(Stream *stream);
		list<Stream*>::iterator GetStreamFirstIt(){return stream_pool.begin();}
		list<Stream*>::iterator GetStreamLastIt(){return stream_pool.end();}
		list<Stream*>::iterator RemoveStreamIt(list<Stream*>::iterator it); 
		unsigned int GetSize(){return stream_pool.size();};
		void ShowStreams();
};

