/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: StreamPool.C,v 5.2 2011-03-23 02:46:07 sin Exp $
*/

#include "StreamPool.H"

StreamPool::StreamPool(){
	return;
}

list<Stream*>::iterator StreamPool::AddStream(Stream *stream){
	return stream_pool.insert(stream_pool.end(), stream);
}

list<Stream*>::iterator StreamPool::RemoveStreamIt(list<Stream*>::iterator it){
	if(it == stream_pool.end() ){return stream_pool.end();}
	return stream_pool.erase(it);
}

void StreamPool::ShowStreams(){
	for(list<Stream*>::iterator it = stream_pool.begin(); it!=stream_pool.end(); it++){
		(*it)->Show();
	}
}
