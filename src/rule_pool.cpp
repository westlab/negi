/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: RulePool.C,v 5.2 2011-01-03 10:22:04 sin Exp $
 */
#include "rule_pool.h"
#include "glog/logging.h"

RulePool::RulePool() {
    return;
}


list<Rule*>::iterator RulePool::GetRuleFirstIt() {
    return rule_pool_.begin();
}

list<Rule*>::iterator RulePool::GetRuleLastIt() {
    return rule_pool_.end();
}

void RulePool::ShowRules() {
    for (list<Rule*>::iterator it = rule_pool_.begin(); it!= rule_pool_.end(); it++) {
        (*it)->Show();
    }
}
void RulePool::ShowResult() {
    for (list<Rule*>::iterator it = rule_pool_.begin(); it!= rule_pool_.end(); it++) {
        (*it)->ShowResult();
    }
}
void RulePool::Clear() {
    for (list<Rule*>::iterator it = rule_pool_.begin(); it!= rule_pool_.end(); it++) {
        delete *it;
    }
    rule_pool_.clear();
}

void RulePool::DeleteLastRule() {
    rule_pool_.pop_back();
}


void RulePool::AddRule(int _id, string _owner, string _timestamp,
                       string _src_ip, string _src_netmask, int _src_port,
                       string _dst_ip, string _dst_netmask, int _dst_port,
                       string _regexp, string _prefilter_pattern, int _prefilter_offset,
                       int _prefilter_depth, int _save_flag) {
    Rule* rule_p = new Rule(_id, _owner, _timestamp,
                            _src_ip, _src_netmask, _src_port,
                            _dst_ip, _dst_netmask, _dst_port,
                            _regexp, _prefilter_pattern, _prefilter_offset,
                            _prefilter_depth, _save_flag);
    rule_pool_.push_back(rule_p);
}

void RulePool::AddRule(int id, string rule) {
    Rule* rule_p = new Rule(id, "", "", "", "", 0, "", "", 0, "", rule, 0, 0, 1);
    LOG(INFO) << rule << " is added";
    rule_pool_.push_back(rule_p);
}
