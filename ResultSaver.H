/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: ResultSaver.H,v 5.0 2010-10-13 08:47:16 sin Exp $
*/
#pragma once
#include "Packet.H"
#include "Include.H"

class ResultSaver
{
	private:

	public:
		ResultSaver();
		~ResultSaver();
		void Proc(Packet *pkt);
};
