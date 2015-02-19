#pragma once

#include "Include.H"
#include "Stream.H"
#include "Packet.H"

class SqliteSaver{
    private:
        char timestamp[100];

    public:
        SqliteSaver();
        void Proc(Stream * stream);
        void ProcPacket(Packet * pkt);
};
