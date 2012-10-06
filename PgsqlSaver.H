/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/* 
 * $Id: PgsqlSaver.H,v 5.0 2010-10-13 08:47:16 sin Exp $
*/

#pragma once

#include "Include.H"
#include "Stream.H"
#include "Packet.H"
#include <pqxx/pqxx>
using namespace pqxx;

class PgsqlSaver
{
	private:
		char timetemp[100];
	public:
		PgsqlSaver();
		void Proc(Stream * stream);
		void ProcPacket(Packet * pkt);
};
