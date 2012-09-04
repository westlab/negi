/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Proc.H,v 5.1 2011-11-08 03:08:17 sin Exp $
*/
#pragma once
#include "Include.H"
#include "Proc.H"
#include "Packet.H"
#include <ext/stdio_filebuf.h>



#ifdef PROC_DEBUG_EN
#define PROC_DEBUG(x)  x ;
#else
#define PROC_DEBUG(x)   ;
#endif


class Proc
{
	private:
		int observer_packet_counter;

	public:
		Proc();
		~Proc();
		void Init();
		void InitPacket();
		void InitManager();
		void InitMaster();

};
