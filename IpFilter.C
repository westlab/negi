/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: IpFilter.C,v 5.0 2010-10-13 08:47:15 sin Exp $
*/

#include "Include.H"
#include "IpFilter.H"

IpFilter::IpFilter(){
	return;
}

void IpFilter::Proc(Packet *pkt){
	Stream *stream = pkt->GetStream();
	if(stream->GetPacketNum() == 1){
		
	}
	return;
}
