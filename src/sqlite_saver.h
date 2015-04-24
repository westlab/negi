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
