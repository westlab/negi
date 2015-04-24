/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: MatchPreFilter.H,v 5.3 2012-03-25 13:21:19 sin Exp $
*/
#pragma once
#include "packet.h"
#include "stream.h"
#include "rule.h"
#include "match_prefilter_info.h"
class Stream;
class Packet;
class Rule;

#ifdef MPF_DEBUG_EN
#define MPF_DEBUG(x)  x ;
#else
#define MPF_DEBUG(x)   ;
#endif
const int MAXN = 10;//max number of patterns at one state
const int MAXS = 200000;//max number of states < the number of characters of patterns
const int MAXC = 256;//ascii code

class ActiveRule{
    friend class MatchPreFilterState;
    friend class MatchPreFilter;
    private:
        // Rule *p_rule;
        list<Rule*>::iterator rule_it_;
        int rule_state_flag_;
        //for kmp
        int kmp_state_;
};

class MatchPreFilterState{
    friend class MatchPreFilter;
    private:
        list<ActiveRule*> active_rule_list_;
        int max_prefilter_pattern_size_;
        int after_ip_filter_;
        int after_pre_filter_;
        string match_pre_filter_log_;

        //for BM
        u_char *temp_buf_;
        //for Aho
        int tmpState_;

    public:
        MatchPreFilterState(Stream *stream);
        ~MatchPreFilterState();
        int GetAfterIpFilter(){return after_ip_filter_;}
        int GetAfterPreFilter(){return after_pre_filter_;}
        string GetMatchPreFilterLog(){return match_pre_filter_log_;}
};

class MatchPreFilter{
    private:
        int **out_;
        int **g_;
        u_char& GetText(int i, u_char *p_content, MatchPreFilterState *state);

#ifndef MATCH_ALL
        int AfterMatch(int mode, int j, MatchPreFilterInfo *match_pre_filter_info, u_char *p_content);
        int Slide(int mode, int j, int i, MatchPreFilterInfo *match_pre_filter_info, u_char *p_content);
        int BoyerMoore(int mode, int start_flag, MatchPreFilterState *state, Packet *packet, int start_place, u_char *p_content, u_char *p_content_end, Rule *rule);
#endif

        MatchPreFilterState * MakeMatchPreFilterState(Stream *stream);
        char buffer_[BUFF_SIZE];

        void initAhoMachine();
        void buildAhoMachine();
        int AhoSearch(
                int mode,
                int start_flag,
                MatchPreFilterState *state,
                Packet *packet,
                int start_place,
                u_char *p_content,
                u_char *p_content_end);
    public:
        MatchPreFilter();
        ~MatchPreFilter();
        int Proc(Packet *packet);
};
