/*
* ** Copyright (C) 2015 Westlab, ALL Rights Reserved.
*/
#ifndef SQLITE_SAVER_H_
#define SQLITE_SAVER_H_

#pragma once

#include "include.h"
#include "stream.h"
#include "packet.h"

class SqliteSaver{
 private:
        char timestamp[100];

 public:
        SqliteSaver();
        void Proc(Stream * stream);
        void ProcPacket(Packet * pkt);
};
#endif  // SQLITE_SAVER_H_
