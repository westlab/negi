/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: StreamRebuild.H,v 5.0 2010-10-13 08:47:16 sin Exp $
*/
#ifndef STREAM_REBUILD_H_
#define STREAM_REBUILD_H_
#pragma once
#include "include.h"
#include "packet.h"
#include "stream.h"
#include "tcp_conn.h"

#ifdef ST_REBUILD_DEBUG_EN
#define ST_REBUILD_DEBUG(x)  x;
#else
#define ST_REBUILD_DEBUG(x);
#endif

class StreamRebuild {
 private:
        void SearchStream(Packet * pkt);
        struct keyvalue {
            struct in_addr src_ip;
            struct in_addr dst_ip;
            unsigned int src_port;
            unsigned int dst_port;
        };

 public:
        StreamRebuild();
        void Proc(Packet *pkt);
        void Init(Packet *pkt);
};
#endif  // STREAM_REBUILD_H_
