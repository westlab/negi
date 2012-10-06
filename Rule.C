/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Rule.C,v 5.3 2011-01-19 05:52:07 sin Exp $
*/
#include "Rule.H"

Rule::Rule(int _id, string _owner, string _timestamp, string _src_ip, string _src_netmask, int _src_port, string _dst_ip, string _dst_netmask, int _dst_port, string _regexp, string _prefilter_pattern, int _prefilter_offset, int _prefilter_depth, int _save_flag){
	id = _id;
	owner = _owner;
	strptime(_timestamp.c_str(),"%Y-%m-%d %H:%M:%S.", &timestamp);
	inet_aton(_src_ip.c_str(), &src_ip);
	inet_aton(_src_netmask.c_str(), &src_netmask);
	inet_aton(_dst_ip.c_str(), &dst_ip);
	inet_aton(_dst_netmask.c_str(), &dst_netmask);

	src_port = _src_port;
	dst_port = _dst_port;


	prefilter_pattern = _prefilter_pattern;
	offset = _prefilter_offset;
	if(_prefilter_depth == 0){
		depth = INT_MAX;
	}else{
		depth = _prefilter_depth;
	}
	regexp = _regexp;
	memcpy(pattern, regexp.c_str(),100);
	pattern[99] = '\0';
	save_flag = _save_flag;

	//engine specific process
	MakeMatchPreFilterInfo();
	return;
}

Rule::~Rule(){
	delete match_pre_filter_info;
	return;
}

void Rule::MakeMatchPreFilterInfo(){
	match_pre_filter_info = new MatchPreFilterInfo(prefilter_pattern);
	return;
}

int Rule::GetRegExpSize(){
	
	return regexp.size();
}

string Rule::GetPreFilterPattern(){
	return prefilter_pattern;
}

int Rule::GetPreFilterOffset(){
	return offset;
}
int Rule::GetPreFilterDepth(){
	return depth;
}


MatchPreFilterInfo* Rule::GetMatchPreFilterInfo(){
	return match_pre_filter_info;
}

void Rule::Show(){
	YELLOW
	cout << "Rule--------------------------" <<endl;
//	cout <<"IP: "<< inet_ntoa(src_ip) << "/" << inet_ntoa(src_netmask) << ":" << src_port;
//	cout << " -> "<<inet_ntoa(dst_ip) << "/" << inet_ntoa(dst_netmask) << ":" << dst_port << endl;
//	cout << "RegExp:" <<regexp<<endl;
	cout << "Pattern:" <<prefilter_pattern<<endl;
//	cout << "PreFilterOffset:" <<offset<<endl;
//	cout << "PreFilterDepth:" <<depth<<endl;
	cout << "------------------------------" <<endl;
	RESET
}
void Rule::ShowResult(){
	match_pre_filter_info->ShowResult();
}
