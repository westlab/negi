/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: MatchPreFilterInfo.C,v 5.1 2010-10-15 11:09:53 harashima Exp $
*/
#include <iomanip>
#include "Include.H"
#include "Global.H"
#include "MatchPreFilterInfo.H"

MatchPreFilterInfo::MatchPreFilterInfo(string _pattern){
	pattern = _pattern;
	pat_len = pattern.size();
	MakeBmBc();

	//for algorithm test
	for(int i = 0; i < STRING_MATCH_NUM; i++){
		match_try[i] = 0;
		read_table[i] = 0;
		match[i] = 0;
	}
}

MatchPreFilterInfo::~MatchPreFilterInfo(){
}

void MatchPreFilterInfo::MakeBmBc(){
	for(int i = 0; i < BM_BC_SIZE; i++){
		bm_bc[i] = pat_len;
		bm_bc_plus[i] = pat_len + 1;
	}
	for(int i = 0; i < pat_len - 1; i++){
		bm_bc[(int)pattern[i]] = pat_len - 1 - i;
		bm_bc_plus[(int)pattern[i]] = pat_len - i;
	}
	bm_bc_plus[(int)pattern[pat_len - 1]] = 1;
}



void MatchPreFilterInfo::ShowResult(){
	for(int i = 0; i < STRING_MATCH_NUM; i++){
		cout << match_try[i] <<",";
	}
	for(int i = 0; i < STRING_MATCH_NUM; i++){
		cout << read_table[i] <<",";
	}
	for(int i = 0; i < STRING_MATCH_NUM; i++){
		cout << match[i] <<",";
	}
	for(int i = 0; i < STRING_MATCH_NUM; i++){
		cout << sum_slide[i] <<",";
	}
	cout << endl;
}
