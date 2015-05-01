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
#include "rule.h"

Rule::Rule(int _id, string _owner, string _timestamp, string _src_ip,
           string _src_netmask, int _src_port, string _dst_ip,
           string _dst_netmask, int _dst_port, string _regexp, string _prefilter_pattern,
           int _prefilter_offset, int _prefilter_depth, int _save_flag) {
    id_ = _id;
    owner_ = _owner;
    strptime(_timestamp.c_str(), "%Y-%m-%d %H:%M:%S.", &timestamp_);
    inet_aton(_src_ip.c_str(), &src_ip_);
    inet_aton(_src_netmask.c_str(), &src_netmask_);
    inet_aton(_dst_ip.c_str(), &dst_ip_);
    inet_aton(_dst_netmask.c_str(), &dst_netmask_);

    src_port_ = _src_port;
    dst_port_ = _dst_port;


    prefilter_pattern_ = _prefilter_pattern;
    offset_ = _prefilter_offset;
    if (_prefilter_depth == 0) {
        depth_ = INT_MAX;
    } else {
        depth_ = _prefilter_depth;
    }
    regexp_ = _regexp;
    memcpy(pattern_, regexp_.c_str(), 100);
    pattern_[99] = '\0';
    save_flag_ = _save_flag;

    // engine specific process
    MakeMatchPreFilterInfo();
    return;
}

Rule::~Rule() {
    delete match_pre_filter_info_;
    return;
}

void Rule::MakeMatchPreFilterInfo() {
    match_pre_filter_info_ = new MatchPreFilterInfo(prefilter_pattern_);
    return;
}

int Rule::GetRegExpSize() {
    return regexp_.size();
}

string Rule::GetPreFilterPattern() {
    return prefilter_pattern_;
}

int Rule::GetPreFilterOffset() {
    return offset_;
}
int Rule::GetPreFilterDepth() {
    return depth_;
}

MatchPreFilterInfo* Rule::GetMatchPreFilterInfo() {
    return match_pre_filter_info_;
}

void Rule::Show() {
    YELLOW
    cout << "Rule--------------------------" <<endl;
// cout <<"IP: "<< inet_ntoa(src_ip) << "/" << inet_ntoa(src_netmask) << ":" << src_port;
// cout << " -> "<<inet_ntoa(dst_ip) << "/" << inet_ntoa(dst_netmask) << ":" << dst_port << endl;
// cout << "RegExp:" <<regexp<<endl;
    cout << "Pattern:" << prefilter_pattern_ <<endl;
// cout << "PreFilterOffset:" <<offset<<endl;
// cout << "PreFilterDepth:" <<depth<<endl;
    cout << "------------------------------" <<endl;
    RESET
}
void Rule::ShowResult() {
    match_pre_filter_info_->ShowResult();
}
