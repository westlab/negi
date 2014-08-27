/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: MatchPreFilter.C,v 5.14 2012-04-30 07:15:01 sin Exp $
*/

#include "Include.H"
#include "MatchPreFilter.H"
#include "Stream.H"
#include "Packet.H"
#include "Rule.H"
#include "Global.H"
#include "Test.H"

//#define MATCH_PRE_FILTER_LOCAL
#define MATCH_ALL

MatchPreFilter::MatchPreFilter(){
	g = new int*[MAXS];
	for(int i=0; i<MAXS; i++){
		g[i] = new int[MAXC];
	}
	out = new int*[MAXS];
	for (int i=0; i<MAXS; i++){
		out[i] = new int[MAXN];
	}
	initAhoMachine();
	buildAhoMachine();
	return;
}

MatchPreFilter::~MatchPreFilter(){
	for(int j=0; j<MAXS; j++){
		delete [] g[j];
	}
	delete [] g;
	for(int j=0; j<MAXS; j++){
		delete [] out[j];
	}
	delete [] out;
	return;
}

void MatchPreFilter::initAhoMachine(){
	for(int i=0; i<MAXS; i++){
		for(int k=0; k<MAXN; k++){
			out[i][k] = 0;
		}
		for(int j=0; j<MAXC; j++){
			g[i][j] = -1;
		}
	}
}

void MatchPreFilter::buildAhoMachine(){
	int *f;
	f = new int[MAXS];
	
	for(int i=0; i<MAXS; i++){
		f[i] = -1;
	}

	int states = 1; //root node's state is 0
	int index = 0;
	int currentState;

    /*
	connection *conn = pgsql->GetConn();
	work T(*conn);
	result *pattern_list;
	pattern_list = new result( T.exec("select prefilter_pattern from rule order by id") );
	//T.commit();
*/
    for(list<Rule*>::iterator rule_it = rule_pool->GetRuleFirstIt(); rule_it != rule_pool->GetRuleLastIt(); rule_it++) {
		string keyword = (*rule_it)->GetPreFilterPattern();
		currentState = 0;
		for (unsigned int j = 0; j < keyword.size(); j++) {
			int c = keyword[j];
			if (g[currentState][c] == -1) { // Allocate a new node
				g[currentState][c] = states++;
			}
			currentState = g[currentState][c];
		}
		out[currentState][0] = index + 1;
		index++;
    }
/*
	for( result::const_iterator it = pattern_list->begin(); it != pattern_list->end(); it++ ){
		string keyword = it[0].as( string() );
		currentState = 0;
		for (unsigned int j = 0; j < keyword.size(); j++) {
			int c = keyword[j];
			if (g[currentState][c] == -1) { // Allocate a new node
				g[currentState][c] = states++;
			}
			currentState = g[currentState][c];
		}
		out[currentState][0] = index + 1;
		index++;
	}
*/
	int StateNum = states - 1;
	//State 0 should have an outgoing edge for all characters.
	for (int c = 0; c < MAXC; ++c) {
		if (g[0][c] == -1) {
			g[0][c] = 0;
		}
	}

	//build the failure function
	queue<int> q;
	for (int c=0; c<MAXC; ++c) {
		if (g[0][c] != 0) {
			f[g[0][c]] = 0;//failure transition must go state0 at state of depth1
			q.push(g[0][c]);
		}
	}
	while (q.size()) {
		int state = q.front();
		q.pop();
		for (int c=0; c<MAXC; ++c) {
			if (g[state][c] != -1) {
				int failure = f[state];
				while (g[failure][c] == -1) {
					failure = f[failure];
				}
				failure = g[failure][c];
				f[g[state][c]] = failure;
				for(int i=0; (i<MAXN && out[failure][i]>0); i++){//Merge all output
					for(int j=0; j<MAXN; j++){
						if(out[g[state][c]][j] == 0){
							out[g[state][c]][j] = out[failure][i];
							break;
						}
					}
				}
				q.push(g[state][c]);
			}
		}
	}
	//make failure function into goto function
	for(int i=0; i<=StateNum; i++){
		for(int j=0; j<MAXC; j++){
			int k = i;
			int c = j;
			while(g[k][c] == -1){
				k = f[k];
			}
			g[i][j] = g[k][c];
		}
	}
	//for escape segmentation fault
	for(int i=0; i<=StateNum; i++){
		for(int j=0;j<MAXC; j++){
			if(g[i][j] == -1){
				g[i][j] = 0;
			}
		}
	}

	delete [] f;

}

int MatchPreFilter::AhoSearch(int mode, int start_flag, MatchPreFilterState *state, Packet *packet, int start_place, u_char *p_content, u_char *p_content_end){
	list<ActiveRule*>::iterator active_rule_it = state->active_rule_list.begin();
	int content_size = (int)packet->GetL7ContentSize();
//	string pattern = rule->GetPreFilterPattern();
	string pattern;
	MatchPreFilterInfo *match_pre_filter_info = (*(*active_rule_it)->rule_it)->GetMatchPreFilterInfo();
//	int pat_len = (int)match_pre_filter_info->pat_len;
	int pat_len;
//    int* bm_bc = match_pre_filter_info->bm_bc;
	int j;//pointer of input data
//cout << "harashima start Packet Size: " << content_size << ", pattern = " << pattern <<endl;

	//for algorithm test
	int* match_try = match_pre_filter_info->match_try;
	int* read_table = match_pre_filter_info->read_table;
	int* match = match_pre_filter_info->match;

	int anterior_content_size = packet->GetStream()->GetL7RetrievedContentSize() - packet->GetL7ContentSize();
	
	int currentState;//for Aho Corasick
	j = start_place;

	if(start_flag == 1){
		//cout << "something start place :" <<start_place << endl;
		if(start_place < 0){
			ERROR_DEBUG(cout << "something error on start place :" <<start_place << endl;)
			return 1;
		}
	//start packet for this rule
		currentState = 0;
	}else{
	//continue packet for this rule
		currentState = state->tmpState;
	}

	while(j < (int)content_size){
		match_try[mode]++;
		currentState = g[currentState][(int)p_content[j]];
		if(out[currentState][0] != 0){
			for(int index=0; (index<MAXN && out[currentState][index]>0); index++){
				for(int tekitoh=0; tekitoh<(out[currentState][index]-1); tekitoh++){ //kokoga dasai
					if(active_rule_it != state->active_rule_list.end())
					active_rule_it++;
				}
				pattern = (*(*active_rule_it)->rule_it)->GetPreFilterPattern();
				pat_len = pattern.size();
				packet->GetStream()->SetSaveFlag();
				match[mode]++;
				if(mode == SUND){
					sprintf(buffer, "%d", anterior_content_size + j - (pat_len-1));
					//sprintf(buffer, "%d", anterior_content_size + j);
					//state->match_pre_filter_log += " [" + pattern + ":" + lexical_cast<string>(anterior_content_size + j) + "]";
					state->match_pre_filter_log += " [" + pattern + ":" + buffer + "]";
					PapaResult* temp_result = new PapaResult;
					temp_result->SetPRule((*(*active_rule_it)->rule_it));
					temp_result->SetRuleId((*(*active_rule_it)->rule_it)->GetId());
					temp_result->SetPatLen(pat_len);
					temp_result->SetPlaceOfPacket(j);//at the end of pattern
					packet->GetStream()->AddPapaResult(temp_result);
//cout << "harashima MSG " << packet->GetStream() <<", "<< rule <<", "<< rule->GetId() <<", "<< pat_len <<", "<< j << endl;
					active_rule_it = state->active_rule_list.begin();
				}
			}
#ifdef MATCH_ALL
			//j += AfterMatch(mode, j, match_pre_filter_info, p_content);
			//j++;
//MSG(mode<<": j="<< j <<", AfterMatch"<<AfterMatch(mode, j, match_pre_filter_info, p_content))
#else

/*
cout << "==============================================BEGIN"<< endl;
for(list<PapaResult*>::iterator it = packet->GetStream()->GetPapaResultListFirstIt(); it != packet->GetStream()->GetPapaResultListLastIt(); it++){
	cout << "this is result!!--------------------" << endl;
	cout << "Stream id: "<< packet->GetStream() << endl;
	cout << "Rule id: "<< (*it)->GetRuleId() << endl;
	cout << "Rule : "<< (*it)->GetPRule()->GetPreFilterPattern() << endl;
	cout << "Pattern Length: "<< (*it)->GetPatLen() << endl;
	cout << "Stream Size: " << packet->GetStream()->GetL7RetrievedContentSize() << endl;
	cout << "Packet Size: " << packet->GetL7ContentSize() << endl;
	cout << "Packet Place: " << (*it)->GetPlaceOfPacket() << endl;
	cout << "Packet Offset: " << (*it)->GetResultOffset() << endl;
	cout << "Flag: " << (*it)->GetFinished() << endl;
}
cout << "==============================================END"<< endl;
*/

			return 1;
#endif
		}else{ //found miss
			read_table[mode]++;
//MSG(mode<<": j="<< j <<", Slide"<<Slide(mode, j, i, match_pre_filter_info, p_content))
		}

		j++;
	}
	state->tmpState = currentState;

	return 0;
}


u_char& MatchPreFilter::GetText(int i, u_char *p_content, MatchPreFilterState *state){
	if(i >= 0){return p_content[i];}
	else{return state->temp_buf[state->max_prefilter_pattern_size + i];}
}

/*
int MatchPreFilter::AfterMatch(int mode, int j, MatchPreFilterInfo *match_pre_filter_info, u_char *p_content){
	int result;
	if(mode == HORS){
		result = (int)match_pre_filter_info->bm_bc[(int)p_content[j + (int)match_pre_filter_info->pat_len - 1]];
	}else if(mode == SUND){
		result = (int)match_pre_filter_info->bm_bc[(int)p_content[j + (int)match_pre_filter_info->pat_len]] + 1;
	}else{
		result = 0;
MSG("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa error desu by MatchPreFilter.C")
	}
	match_pre_filter_info->sum_slide[mode] += result;
	return result;
}

int MatchPreFilter::Slide(int mode, int j, int i, MatchPreFilterInfo *match_pre_filter_info, u_char *p_content){
	int result;
	if(mode == HORS){
		result = (int)match_pre_filter_info->bm_bc[(int)p_content[j + (int)match_pre_filter_info->pat_len - 1]];
	}else if(mode == SUND){
		result = (int)match_pre_filter_info->bm_bc_plus[(int)p_content[j + (int)match_pre_filter_info->pat_len]];
	}else{
		result = 0;
MSG("mode can be 1 to 7")
	}
	match_pre_filter_info->sum_slide[mode] += result;
	return result;
}

int MatchPreFilter::BoyerMoore(int mode, int start_flag, MatchPreFilterState *state, Packet *packet, int start_place, u_char *p_content, u_char *p_content_end, Rule *rule){
	int content_size = (int)packet->GetL7ContentSize();
	string pattern = rule->GetPreFilterPattern();
	MatchPreFilterInfo *match_pre_filter_info = rule->GetMatchPreFilterInfo();
	int pat_len = (int)match_pre_filter_info->pat_len;
//    int* bm_bc = match_pre_filter_info->bm_bc;
	int i, j;
//cout << "harashima start Packet Size: " << content_size << ", pattern = " << pattern <<endl;

	//for algorithm test
	int* match_try = match_pre_filter_info->match_try;
	int* read_table = match_pre_filter_info->read_table;
	int* match = match_pre_filter_info->match;

	int anterior_content_size = packet->GetStream()->GetL7RetrievedContentSize() - packet->GetL7ContentSize();

	if(start_flag == 1){
		//cout << "something start place :" <<start_place << endl;
		if(start_place < 0){
			ERROR_DEBUG(cout << "something error on start place :" <<start_place << endl;)
			return 1;
		}
	//start packet for this rule
		j = start_place;
	}else{
	//continue packet for this rule
		j = 1 - pat_len;
		//tsunagi mode
		while(j < 0){
//MSG("tsuna: j="<< j)
			for(i = pat_len - 1; i >= 0 && (match_try[mode]++, pattern[i] == GetText(j + i, p_content, state)); i--){
//MSG(mode<<": j="<< j <<", ------------"<<p_content[j+i])
			}
			if(i < 0){ //matched!!
				packet->GetStream()->SetSaveFlag();
				match[mode]++;
				if(mode == SUND){
					sprintf(buffer, "%d", anterior_content_size + j);
					//state->match_pre_filter_log += " [" + pattern + ":" + lexical_cast<string>(anterior_content_size + j) + "]";
					state->match_pre_filter_log += " [" + pattern + ":" + buffer + "]";
					PapaResult* temp_result = new PapaResult;
					temp_result->SetPRule(rule);
					temp_result->SetRuleId(rule->GetId());
					temp_result->SetPatLen(pat_len);
					temp_result->SetPlaceOfPacket(j);
					packet->GetStream()->AddPapaResult(temp_result);
//cout << "harashima tsunagi Packet Size: " << packet->GetL7ContentSize() << endl;
				}
#ifdef MATCH_ALL
				j += AfterMatch(mode, j, match_pre_filter_info, p_content);
#else
				return 1;
#endif
			}else{ //found miss
				read_table[mode]++;
				j += Slide(mode, j, i, match_pre_filter_info, p_content);
			}
		}
	}

	//nomal mode
//    while(j <= (int)content_size - pat_len){
	while(j < (int)content_size - pat_len){
		for(i = pat_len - 1; i >= 0 && (match_try[mode]++, pattern[i] == p_content[j + i]); i--){
//MSG(mode<<": j="<< j <<", ------------"<<p_content[j+i])
		}
		if(i < 0){ //matched!!
			packet->GetStream()->SetSaveFlag();
			match[mode]++;
			if(mode == SUND){
				sprintf(buffer, "%d", anterior_content_size + j);
				//state->match_pre_filter_log += " [" + pattern + ":" + lexical_cast<string>(anterior_content_size + j) + "]";
				state->match_pre_filter_log += " [" + pattern + ":" + buffer + "]";
				PapaResult* temp_result = new PapaResult;
				temp_result->SetPRule(rule);
				temp_result->SetRuleId(rule->GetId());
				temp_result->SetPatLen(pat_len);
				temp_result->SetPlaceOfPacket(j);
				packet->GetStream()->AddPapaResult(temp_result);
//cout << "harashima MSG " << packet->GetStream() <<", "<< rule <<", "<< rule->GetId() <<", "<< pat_len <<", "<< j << endl;
			}
#ifdef MATCH_ALL
			j += AfterMatch(mode, j, match_pre_filter_info, p_content);
//MSG(mode<<": j="<< j <<", AfterMatch"<<AfterMatch(mode, j, match_pre_filter_info, p_content))
#else


//cout << "==============================================BEGIN"<< endl;
//for(list<PapaResult*>::iterator it = packet->GetStream()->GetPapaResultListFirstIt(); it != packet->GetStream()->GetPapaResultListLastIt(); it++){
//	cout << "this is result!!--------------------" << endl;
//	cout << "Stream id: "<< packet->GetStream() << endl;
//	cout << "Rule id: "<< (*it)->GetRuleId() << endl;
//	cout << "Rule : "<< (*it)->GetPRule()->GetPreFilterPattern() << endl;
//	cout << "Pattern Length: "<< (*it)->GetPatLen() << endl;
//	cout << "Stream Size: " << packet->GetStream()->GetL7RetrievedContentSize() << endl;
//	cout << "Packet Size: " << packet->GetL7ContentSize() << endl;
//	cout << "Packet Place: " << (*it)->GetPlaceOfPacket() << endl;
//	cout << "Packet Offset: " << (*it)->GetResultOffset() << endl;
//	cout << "Flag: " << (*it)->GetFinished() << endl;
//}
//cout << "==============================================END"<< endl;


			return 1;
#endif
		}else{ //found miss
			read_table[mode]++;
			j += Slide(mode, j, i, match_pre_filter_info, p_content);
//MSG(mode<<": j="<< j <<", Slide"<<Slide(mode, j, i, match_pre_filter_info, p_content))
		}
	}
	return 0;
}
*/

MatchPreFilterState * MatchPreFilter::MakeMatchPreFilterState(Stream *stream){
	MatchPreFilterState *state = new MatchPreFilterState(stream);
	observer->MPFStateMallocd(sizeof(*state));
	OBSERVER_DEBUG(BLUE cout << "MPFStateMallocd :" << sizeof(*state) << endl;RESET);
	stream->SetMatchPreFilterState(state);
	return state;
}

int MatchPreFilter::Proc(Packet *packet){ //suppose that don't seatch 2 packet before.
MPF_DEBUG(MSG("MPFilter: ----------process start----------"<<packet->GetL7ContentSize()))
	if(packet->GetProtocol() != IPPROTO_TCP){
//        MSG("MPFilter: Protocol is not TCP")
			return 0;
	}
	if(packet->GetL7ContentSize() == 0){
//        MSG("MPFilter: ContentSize is 0")
			return 0;
	}
	if(packet->GetStream() == NULL){
//        MSG("MPFilter: packet->stream is NULL")
			return 0;
	}

	Stream *stream = packet->GetStream();
//	MSG(packet->GetL7Content());
	MatchPreFilterState *state;
	if(stream->GetMatchPreFilterState() == 0){
		state = MakeMatchPreFilterState(stream);
	}else{
		state = stream->GetMatchPreFilterState();
	}
	int after_pre_filter = 0;
	int retrieved_content_size = stream->GetL7RetrievedContentSize();
	int content_size = packet->GetL7ContentSize();
	
	if(stream->GetPacketNum() == 1 && retrieved_content_size != content_size){
	cout << "------------------" << endl;
	cout << "Harashima PacketNum:" <<  stream->GetPacketNum() << " retrieved_content_size: " << retrieved_content_size << " content_size: " << content_size << endl;
	}

//MSG("MPFilter: ----------process start----------"<<packet->GetL7ContentSize())
	//single pattern
	list<ActiveRule*>::iterator active_rule_it = state->active_rule_list.begin();
//	while(active_rule_it != state->active_rule_list.end()){
		int start_flag = 0;
//unused!!
//		int end_flag = 0;
		int offset = (*(*active_rule_it)->rule_it)->GetPreFilterOffset();
		int depth = (*(*active_rule_it)->rule_it)->GetPreFilterDepth();
		if(depth == 0){
			depth = INT_MAX;
		}
		int start = offset;
		int end = start + depth - 1;
		u_char *p_content = NULL, *p_content_end = NULL;
		int start_place = 0;
//cout << "harashima start Packet Size: " << packet->GetL7ContentSize()<< ", pattern = " << (*(*active_rule_it)->rule_it)->GetPreFilterPattern() << ", flag = " << (*active_rule_it)->rule_state_flag <<endl;

//		if((*active_rule_it)->rule_state_flag == 0 || (*active_rule_it)->rule_state_flag == 1){
//		cout << "------------------" << endl;
//		cout << "start: " << start << " retrieved_content_size: " << retrieved_content_size << " content_size: " << content_size << endl;
//		cout << "rule_state_flag: " <<(*active_rule_it)->rule_state_flag << endl;
//		}

		if((*active_rule_it)->rule_state_flag == 3){
			after_pre_filter++;
		}
		//check this packet conteints start byte
		if((*active_rule_it)->rule_state_flag == 0){
			if(start <= retrieved_content_size - 1){ //start packet
				start_flag = 1;
				(*active_rule_it)->rule_state_flag = 1;

				start_place = start - (retrieved_content_size - content_size);
				p_content = packet->GetL7Content() + start_place;
			}
		}

		if((*active_rule_it)->rule_state_flag == 1){
			if(start_flag == 0){
				start_place = 0;
				p_content = packet->GetL7Content();
			}
			//check this packet conteints end byte
			if(end <= retrieved_content_size - 1){ //end packet    korenani?
//unused!!
//				end_flag = 1;
				p_content_end = packet->GetL7Content() + end - (retrieved_content_size - content_size);
#ifdef MATCH_ALL
////				BoyerMoore(HORS, start_flag, state, packet, start_place, p_content, p_content_end, *(*active_rule_it)->rule_it);
				//BoyerMoore(SUND, start_flag, state, packet, start_place, p_content, p_content_end, *(*active_rule_it)->rule_it);
				AhoSearch(SUND, start_flag, state, packet, start_place, p_content, p_content_end);
//				MPF_DEBUG(BLUE cout << "-----last packet for this rule-----" << endl;RESET)
				(*active_rule_it)->rule_state_flag = 2;

#else //MATCH_ALL

//				BoyerMoore(HORS, start_flag, state, packet, start_place, p_content, p_content_end, *(*active_rule_it)->rule_it);
				//if(BoyerMoore(SUND, start_flag, state, packet, start_place, p_content, p_content_end, *(*active_rule_it)->rule_it)){
				if(AhoSearch(SUND, start_flag, state, packet, start_place, p_content, p_content_end)){
					(*active_rule_it)->rule_state_flag = 3;
					after_pre_filter++;
					MPF_DEBUG(((*(*active_rule_it)->rule_it)->GetMatchPreFilterInfo())->ShowResult());
				}else{
					(*active_rule_it)->rule_state_flag = 2;
					MPF_DEBUG(((*(*active_rule_it)->rule_it)->GetMatchPreFilterInfo())->ShowResult());
//					stream->RemoveRuleIt((*active_rule_it)->rule_it);
//					delete *active_rule_it;
					observer->MPFActiveRuleDeleted(sizeof(**active_rule_it));
					OBSERVER_DEBUG(BLUE cout << "MPFActiveRuleDeleted :" << sizeof(**active_rule_it) << endl;RESET);
//					list<ActiveRule*>::iterator it = active_rule_it;
					//active_rule_it++;
//					state->active_rule_list.erase(it);
//					continue;
				}
#endif //MATCH_ALL


			}else{ //not end packet
				p_content_end = packet->GetL7Content() + content_size - 1;
#ifdef MATCH_ALL
////				BoyerMoore(HORS, start_flag, state, packet, start_place, p_content, p_content_end, *(*active_rule_it)->rule_it);
				//BoyerMoore(SUND, start_flag, state, packet, start_place, p_content, p_content_end, *(*active_rule_it)->rule_it);
				AhoSearch(SUND, start_flag, state, packet, start_place, p_content, p_content_end);
//
#else //MATCH_ALL
//				BoyerMoore(HORS, start_flag, state, packet, start_place, p_content, p_content_end, *(*active_rule_it)->rule_it);
				//if(BoyerMoore(SUND, start_flag, state, packet, start_place, p_content, p_content_end, *(*active_rule_it)->rule_it)){
				if(AhoSearch(SUND, start_flag, state, packet, start_place, p_content, p_content_end)){
					(*active_rule_it)->rule_state_flag = 3;
					after_pre_filter++;
				}
#endif
			}
		}
		MPF_DEBUG((*(*active_rule_it)->rule_it)->GetMatchPreFilterInfo()->ShowResult());
		//active_rule_it++;
//	}

	//for BoyerMoore
	//size_t copy_size = (size_t)state->max_prefilter_pattern_size;
	//memcpy(state->temp_buf, packet->GetL7Content() +  content_size - copy_size, copy_size);

#ifndef MATCH_ALL
	state->after_pre_filter = after_pre_filter;
//MSG("cyu------moku:  " << stream->GetMatchPreFilterState()->GetAfterIpFilter() << " ---> " << stream->GetMatchPreFilterState()->GetAfterPreFilter())
#endif
//MSG("MPFilter: ----------process end----------"<<packet->GetL7ContentSize())
	return 0;
}


MatchPreFilterState::MatchPreFilterState(Stream *stream){
	max_prefilter_pattern_size = 0;
	size_t active_rule_size= 0;

	for(list<Rule*>::iterator rule_it = stream->GetRuleFirstIt(); rule_it != stream->GetRuleLastIt(); rule_it++){
		ActiveRule* temp_rule = new ActiveRule;
		active_rule_size += sizeof(*temp_rule);
		temp_rule->rule_it = rule_it;
//        temp_rule->rule_id= dd;
		temp_rule->rule_state_flag = 0;
		temp_rule->kmp_state = 0;
		active_rule_list.push_back(temp_rule);
		int pat_len = ((*rule_it)->GetMatchPreFilterInfo())->pat_len;
		if(max_prefilter_pattern_size < pat_len){
			max_prefilter_pattern_size = pat_len;
		}
	}
	observer->MPFActiveRuleMallocd(active_rule_size);
	OBSERVER_DEBUG(BLUE cout << "MPFActiveRuleMallocd :" << active_rule_size << endl;RESET);

	after_ip_filter = active_rule_list.size();
	after_pre_filter = -1;
	temp_buf = new u_char[max_prefilter_pattern_size + 1];
	observer->MPFTempBufMallocd(max_prefilter_pattern_size * sizeof(unsigned char));
	OBSERVER_DEBUG(BLUE cout << "MPFTempBufMallocd :" << max_prefilter_pattern_size * sizeof(unsigned char) << endl;RESET);
}


MatchPreFilterState::~MatchPreFilterState(){
	for(list<ActiveRule*>::iterator rule_it = active_rule_list.begin(); rule_it != active_rule_list.end(); rule_it++){
		observer->MPFActiveRuleDeleted(sizeof(**rule_it));
		OBSERVER_DEBUG(BLUE cout << "MPFActiveRuleDeleted :" << sizeof(**rule_it) << endl;RESET);
		delete *rule_it;
	}
	delete [] temp_buf;
	observer->MPFTempBufDeleted(max_prefilter_pattern_size * sizeof(unsigned char));
	OBSERVER_DEBUG(BLUE cout << "MPFTempBufDeleted :" << max_prefilter_pattern_size * sizeof(unsigned char) << endl;RESET);
}


#ifdef MATCH_PRE_FILTER_LOCAL

int main(){
	new Test();
	return 0;
}

#endif //MATCH_PRE_FILTER_LOCAL

