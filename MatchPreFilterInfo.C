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
//    for(int i = 0; i < 0x100; i++){
//	for(int i = 48; i <= 122; i++){
//		cout << (char)i << " = " << bm_bc[i] << endl;
//	}
//    for(int i = 0; i < (int)pat_len; i++){
//        cout << i << " = " << pattern[i] << " = " << bm_bc[(int)pattern[i]] << endl;
//    }
}



void MatchPreFilterInfo::ShowResult(){
//    cout << pattern <<",match_try,";
//    cout <<"match_try,";
	for(int i = 0; i < STRING_MATCH_NUM; i++){
		cout << match_try[i] <<",";
	}
//    cout <<"read_table,";
	for(int i = 0; i < STRING_MATCH_NUM; i++){
		cout << read_table[i] <<",";
	}
//    cout <<"match,";
	for(int i = 0; i < STRING_MATCH_NUM; i++){
		cout << match[i] <<",";
	}
//    cout <<"sum_slide,";
	for(int i = 0; i < STRING_MATCH_NUM; i++){
		cout << sum_slide[i] <<",";
	}
	cout << endl;
//    YELLOW cout << "-----MatchPreFilterResult-----" << endl;RESET
//MSG(pattern)
//    int i;
//    i = 0, cout << "KMP: try = " << match_try[i] << ", read_table = " << read_table[i] << ", match = " << match[i] << endl;
//    i = 1, cout << "BM1: try = " << match_try[i] << ", read_table = " << read_table[i] << ", match = " << match[i] << endl;
//    i = 2, cout << "BM2: try = " << match_try[i] << ", read_table = " << read_table[i] << ", match = " << match[i] << endl;
//    i = 3, cout << "HAR: try = " << match_try[i] << ", read_table = " << read_table[i] << ", match = " << match[i] << endl;
}
