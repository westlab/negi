/*
 * Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 *
 * Please see the LICENSE file distributed with this source
 * code archive for information covering the modification and
 * redistribution of this file and binaries built from it.
 */

/*
 * $Id: RulePool.H,v 5.2 2011-01-03 10:22:04 sin Exp $
*/
#ifndef RULE_POOL_H_
#define RULE_POOL_H_
#pragma once
#include "include.h"
#include "stream_rebuild.h"
#include "rule.h"

class RulePool{
 private:
        list<Rule *> rule_pool_;
        time_t update_time_;

 public:
        RulePool();
        void Clear();
        void AddRule(int _id, string _owner, string _timestamp,
                     string _src_ip, string _src_netmask, int _src_port,
                     string _dst_ip, string _dst_netmask, int _dst_port,
                     string _regexp, string _prefilter_pattern,
                     int _prefilter_offset, int _prefilter_depth, int _save_flag);
        void AddRule(int id, string rule);
        list<Rule*>::iterator GetRuleFirstIt();
        list<Rule*>::iterator GetRuleLastIt();
        void ShowRules();
        void ShowResult();
        void DeleteLastRule();
};
#endif  // RULE_POOL_H_
