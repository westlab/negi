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
#ifndef STREAM_POOL_H_
#define STREAM_POOL_H_
#pragma once

#include "include.h"
#include "stream.h"
#include "stream_pool.h"

class StreamPool {
 private:
  list<Stream*> stream_pool_;

 public:
  StreamPool();

  list<Stream*>::iterator AddStream(Stream *stream);
  list<Stream*>::iterator GetStreamFirstIt() {return stream_pool_.begin();}
  list<Stream*>::iterator GetStreamLastIt() {return stream_pool_.end();}
  list<Stream*>::iterator RemoveStreamIt(list<Stream*>::iterator it);
  unsigned int GetSize() {return stream_pool_.size();}
  void ShowStreams();
};
#endif  // STREAM_POOL_H_
