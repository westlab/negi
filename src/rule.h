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
#ifndef RULE_H_
#define RULE_H_
#pragma once
#include "include.h"
#include "rule.h"
#include "match_prefilter_info.h"

class Rule {
 private:
  int id_;
  string owner_;
  struct tm timestamp_;
  struct in_addr src_ip_, src_netmask_, dst_ip_, dst_netmask_;
  int src_port_, dst_port_;

  string regexp_;
  char pattern_[100];

  string prefilter_pattern_;
  int offset_, depth_;
  int save_flag_;

  MatchPreFilterInfo *match_pre_filter_info_;

 public:
  Rule(int _id, string _owner, string _timestamp, string _src_ip,
       string _src_netmask, int _src_port, string _dst_ip, string _dst_netmask,
       int _dst_port, string _regexp, string _prefilter_pattern, int _prefilter_offset,
       int _prefilter_depth, int _save_flag);
  ~Rule();
  int GetRegExpSize();
  string GetPreFilterPattern();
  int GetPreFilterOffset();
  int GetPreFilterDepth();
  int GetId() {return id_;}
  int GetSaveFlag() {return save_flag_;}

  void Show();
  void ShowResult();


  void MakeMatchPreFilterInfo();
  MatchPreFilterInfo *GetMatchPreFilterInfo();
};
#endif  // RULE_H_
