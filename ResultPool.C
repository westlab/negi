/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: ResultPool.C,v 5.0 2010-10-13 08:47:16 sin Exp $
*/
#include "ResultPool.H"
#include "Global.H"

ResultPool::ResultPool(){

}

ResultPool::~ResultPool(){
}

void ResultPool::AddResult(Stream *pstream, unsigned int fid, unsigned long int sof_size, unsigned int poffset_size, unsigned int pcontent_size, unsigned char* r_string){
	result_pool.push_back(new Result(pstream, fid, sof_size, poffset_size, pcontent_size, r_string));

}
