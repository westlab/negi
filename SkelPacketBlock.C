/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: SkelPacketBlock.C,v 5.0 2010-10-13 08:47:16 sin Exp $
*/

#include "Include.H"
#include "Packet.H"
#include "BasePacketBlock.H"
#include "SkelPacketBlock.H"

int SkelPacketBlock::proc(Packet *packet, Stream *stream){
	//u_char* hoge = packet->GetL3Header();

	return 0;
}
