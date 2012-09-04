/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: ResultPool.H,v 5.0 2010-10-13 08:47:16 sin Exp $
*/
#pragma once
#include "Packet.H"
#include "Include.H"
#include "Result.H"

class ResultPool
{
	private:
		list<Result*> result_pool;

	public:
		ResultPool();
		~ResultPool();
		void AddResult(Stream *pstream, unsigned int fid, unsigned long int sof_size, unsigned int poffset_size, unsigned int pcontent_size, unsigned char* r_string);
		list<Result *>::iterator GetResultFirstIt(){return result_pool.begin();};
		list<Result *>::iterator GetResultLastIt(){return result_pool.end();};
};
