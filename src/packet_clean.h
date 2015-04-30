/*
 * Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 *
 * Please see the LICENSE file distributed with this source
 * code archive for information covering the modification and
 * redistribution of this file and binaries built from it.
 */

/*
 * $Id: PacketClean.H,v 5.0 2010-10-13 08:47:16 sin Exp $
*/
#ifndef PACKET_CLEAN_H_
#define PACKET_CLEAN_H_
#pragma once
#include "include.h"
#include "packet.h"

class PacketClean {
 private:
        struct timeval last_gc_time_;

 public:
        PacketClean();
        ~PacketClean();
        void Proc(Packet *pkt);
};
#endif  // PACKET_CLEAN_H_
