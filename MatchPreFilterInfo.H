/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: MatchPreFilterInfo.H,v 5.1 2010-10-15 11:09:53 harashima Exp $
*/
#pragma once
#include "Include.H"
#define BM_BC_SIZE 256
//#define BM_BC_SIZE 128
class MatchPreFilterInfo{
	private:
	public:
		string pattern;
		int pat_len;
		int bm_bc[BM_BC_SIZE]; //bad character rule
		int bm_bc_plus[BM_BC_SIZE]; //for Sunday

		//for algorithm test
		int match_try[STRING_MATCH_NUM];
		int read_table[STRING_MATCH_NUM];
		int sum_slide[STRING_MATCH_NUM];
		int match[STRING_MATCH_NUM];

		MatchPreFilterInfo(string pattern);
		~MatchPreFilterInfo();
		void MakeBmBc();
		void ShowResult();

};

