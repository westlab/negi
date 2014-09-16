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
#include "BasePacketBlock.H"
#include "Packet.H"
#include "Stream.H"
#include "Rule.H"
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
//        Rule *p_rule;
		list<Rule*>::iterator rule_it;
		int rule_state_flag;
		//for kmp
		int kmp_state;
};

class MatchPreFilterState{
	friend class MatchPreFilter;
	private:
		list<ActiveRule*> active_rule_list;
		int max_prefilter_pattern_size;
		int after_ip_filter;
		int after_pre_filter;
		string match_pre_filter_log;

		//for BM
		u_char *temp_buf;
		//for Aho
		int tmpState;

	public:
		MatchPreFilterState(Stream *stream);
		~MatchPreFilterState();
		int GetAfterIpFilter(){return after_ip_filter;}
		int GetAfterPreFilter(){return after_pre_filter;}
		string GetMatchPreFilterLog(){return match_pre_filter_log;}
};

class MatchPreFilter{
	private:
		int **out;
		int **g;
		u_char& GetText(int i, u_char *p_content, MatchPreFilterState *state);
		
		/*
		int AfterMatch(int mode, int j, MatchPreFilterInfo *match_pre_filter_info, u_char *p_content);
		int Slide(int mode, int j, int i, MatchPreFilterInfo *match_pre_filter_info, u_char *p_content);
		int BoyerMoore(int mode, int start_flag, MatchPreFilterState *state, Packet *packet, int start_place, u_char *p_content, u_char *p_content_end, Rule *rule);
		*/

		MatchPreFilterState * MakeMatchPreFilterState(Stream *stream);
		char buffer[BUFF_SIZE];
		
		void initAhoMachine();
		void buildAhoMachine();
		int AhoSearch(int mode, int start_flag, MatchPreFilterState *state, Packet *packet, int start_place, u_char *p_content, u_char *p_content_end);
	public:
		MatchPreFilter();
		~MatchPreFilter();
		int Proc(Packet *packet);
};
