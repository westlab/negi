/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Rule.H,v 5.2 2011-01-03 10:22:04 sin Exp $
*/
#pragma once
#include "Include.H"
#include "Rule.H"
#include "MatchPreFilterInfo.H"

class Rule {
	private:
		int id;
		string owner;
		struct tm timestamp;
		struct in_addr src_ip, src_netmask, dst_ip, dst_netmask;
		int src_port, dst_port;

		string regexp;
		char pattern[100];

		string prefilter_pattern;
		int offset, depth;
		int save_flag;
		
		MatchPreFilterInfo *match_pre_filter_info;

		
	public:
		Rule(int _id, string _owner, string _timestamp, string _src_ip, string _src_netmask, int _src_port, string _dst_ip, string _dst_netmask, int _dst_port, string _regexp, string _prefilter_pattern, int _prefilter_offset, int _prefilter_depth, int _save_flag);
		~Rule();
		int GetRegExpSize();
		string GetPreFilterPattern();
		int GetPreFilterOffset();
		int GetPreFilterDepth();
		int GetId(){return id;}
		int GetSaveFlag(){return save_flag;}

		void Show();
		void ShowResult();


		void MakeMatchPreFilterInfo();
		MatchPreFilterInfo *GetMatchPreFilterInfo();
};


