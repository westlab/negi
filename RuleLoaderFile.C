/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: RuleLoaderFile.C,v 5.2 2012/05/23 05:23:48 sin Exp $
*/

#include "Include.H"
#include "Global.H"
#include "Stream.H"

#ifndef USE_POSTGRES

#include "RuleLoaderFile.H"

RuleLoaderFile::RuleLoaderFile(){
	return;
}


int RuleLoaderFile::Proc(){
	rule_pool->Clear();

/*
		for( result::const_iterator c = res.begin(); c != res.end(); ++c ) {
			rule_pool->AddRule(	c[0].as(u_int()), c[1].as(string()), c[2].as(string()), c[3].as(string()), \
			c[4].as(string()), c[5].as(u_int()), c[6].as(string()), c[7].as(string()), c[8].as(u_int()), \
			c[9].as(string()),c[10].as(string()),c[11].as(u_int()), c[12].as(u_int()), c[13].as(u_int()));

		}

*/
	stream_id_prev = 0;

	

	return 0;	
}

#endif //USE_POSTGRES
