/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: IpFilter.H,v 5.0 2010-10-13 08:47:15 sin Exp $
*/
#pragma once

#include "packet.h"

class IpFilter
{
    private:

    public:
        IpFilter();
        void Proc(Packet *pkt);
};
