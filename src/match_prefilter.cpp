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

#include "include.h"
#include "match_prefilter.h"
#include "global.h"
#include "glog/logging.h"

#define MATCH_ALL

MatchPreFilter::MatchPreFilter(){
    g_ = new int*[MAXS];
    for(int i=0; i<MAXS; i++){
        g_[i] = new int[MAXC];
    }
    out_ = new int*[MAXS];
    for (int i=0; i<MAXS; i++){
        out_[i] = new int[MAXN];
    }
    initAhoMachine();
    buildAhoMachine();
    return;
}

MatchPreFilter::~MatchPreFilter(){
    for(int j=0; j<MAXS; j++){
        delete [] g_[j];
    }
    delete [] g_;
    for(int j=0; j<MAXS; j++){
        delete [] out_[j];
    }
    delete [] out_;
    return;
}

void MatchPreFilter::initAhoMachine(){
    for(int i=0; i<MAXS; i++){
        for(int k=0; k<MAXN; k++){
            out_[i][k] = 0;
        }
        for(int j=0; j<MAXC; j++){
            g_[i][j] = -1;
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

    for(list<Rule*>::iterator rule_it = rule_pool->GetRuleFirstIt();
        rule_it != rule_pool->GetRuleLastIt(); rule_it++) {
        string keyword = (*rule_it)->GetPreFilterPattern();
        currentState = 0;
        for (unsigned int j = 0; j < keyword.size(); j++) {
            int c = keyword[j];
            if (g_[currentState][c] == -1) { // Allocate a new node
                g_[currentState][c] = states++;
            }
            currentState = g_[currentState][c];
        }
        out_[currentState][0] = index + 1;
        index++;
    }

    int StateNum = states - 1;
    //State 0 should have an outgoing edge for all characters.
    for (int c = 0; c < MAXC; ++c) {
        if (g_[0][c] == -1) {
            g_[0][c] = 0;
        }
    }

    //build the failure function
    queue<int> q;
    for (int c=0; c<MAXC; ++c) {
        if (g_[0][c] != 0) {
            //failure transition must go state0 at state of depth1
            f[g_[0][c]] = 0;
            q.push(g_[0][c]);
        }
    }
    while (q.size()) {
        int state = q.front();
        q.pop();
        for (int c=0; c<MAXC; ++c) {
            if (g_[state][c] != -1) {
                int failure = f[state];
                while (g_[failure][c] == -1) {
                    failure = f[failure];
                }
                failure = g_[failure][c];
                f[g_[state][c]] = failure;
                for(int i=0; (i<MAXN && out_[failure][i]>0); i++){//Merge all output
                    for(int j=0; j<MAXN; j++){
                        if(out_[g_[state][c]][j] == 0){
                            out_[g_[state][c]][j] = out_[failure][i];
                            break;
                        }
                    }
                }
                q.push(g_[state][c]);
            }
        }
    }
    //make failure function into goto function
    for(int i=0; i<=StateNum; i++){
        for(int j=0; j<MAXC; j++){
            int k = i;
            int c = j;
            while(g_[k][c] == -1){
                k = f[k];
            }
            g_[i][j] = g_[k][c];
        }
    }
    //for escape segmentation fault
    for(int i=0; i<=StateNum; i++){
        for(int j=0;j<MAXC; j++){
            if(g_[i][j] == -1){
                g_[i][j] = 0;
            }
        }
    }
    delete [] f;
}

int MatchPreFilter::AhoSearch(int mode,
                              int start_flag,
                              MatchPreFilterState *state, Packet *packet,
                              int start_place,
                              u_char *p_content,
                              u_char *p_content_end){
    list<ActiveRule*>::iterator active_rule_it = state->active_rule_list_.begin();
    int content_size = (int)packet->GetL7ContentSize();
    string pattern;
    MatchPreFilterInfo *match_pre_filter_info = (*(*active_rule_it)->rule_it_)->GetMatchPreFilterInfo();
    int pat_len;
    int j;//pointer of input data

    //for algorithm test
    int* match_try = match_pre_filter_info->match_try;
    int* read_table = match_pre_filter_info->read_table;
    int* match = match_pre_filter_info->match;

    int anterior_content_size = packet->GetStream()->GetL7RetrievedContentSize() - packet->GetL7ContentSize();

    int currentState;//for Aho Corasick
    j = start_place;

    if(start_flag == 1){
        if(start_place < 0){
            LOG(ERROR) << "Error on start palce";
            LOG(ERROR) << "start palce: " << start_place;
            return 1;
        }
        //start packet for this rule
        currentState = 0;
    }else{
        //continue packet for this rule
        currentState = state->tmpState_;
    }

    while(j < (int)content_size){
        match_try[mode]++;
        currentState = g_[currentState][(int)p_content[j]];
        if(out_[currentState][0] != 0){
            for(int index=0; (index<MAXN && out_[currentState][index]>0); index++){
                for(int k=0; k<(out_[currentState][index]-1); k++){
                    if(active_rule_it != state->active_rule_list_.end()){
                        active_rule_it++;
                    }
                }
                pattern = (*(*active_rule_it)->rule_it_)->GetPreFilterPattern();
                pat_len = pattern.size();
                packet->GetStream()->SetSaveFlag();
                match[mode]++;
                if(mode == SUND){
                    sprintf(buffer_, "%d", anterior_content_size + j - (pat_len-1));
                    state->match_pre_filter_log_ += " [" + pattern + ":" + buffer_ + "]";
                    PapaResult* temp_result = new PapaResult;
                    temp_result->SetPRule((*(*active_rule_it)->rule_it_));
                    temp_result->SetRuleId((*(*active_rule_it)->rule_it_)->GetId());
                    temp_result->SetPatLen(pat_len);
                    temp_result->SetPlaceOfPacket(j);//at the end of pattern
                    packet->GetStream()->AddPapaResult(temp_result);
                    active_rule_it = state->active_rule_list_.begin();
                }
            }
            //j += AfterMatch(mode, j, match_pre_filter_info, p_content);
            //j++;
            //MSG(mode<<": j="<< j <<", AfterMatch"<<AfterMatch(mode, j, match_pre_filter_info, p_content))
        }else{ //found miss
            read_table[mode]++;
            //MSG(mode<<": j="<< j <<", Slide"<<Slide(mode, j, i, match_pre_filter_info, p_content))
        }

        j++;
    }
    state->tmpState_ = currentState;

    return 0;
}

u_char& MatchPreFilter::GetText(int i, u_char *p_content, MatchPreFilterState *state){
    if(i >= 0){return p_content[i];}
    else{return state->temp_buf_[state->max_prefilter_pattern_size_ + i];}
}


MatchPreFilterState * MatchPreFilter::MakeMatchPreFilterState(Stream *stream){
    MatchPreFilterState *state = new MatchPreFilterState(stream);
    observer->MPFStateMallocd(sizeof(*state));
    LOG(INFO) << "MPFStateMallocd :" << sizeof(*state);
    stream->SetMatchPreFilterState(state);
    return state;
}

int MatchPreFilter::Proc(Packet *packet){
    //suppose that don't search 2 packet before.
    LOG(INFO) << "start: " << packet->GetL7ContentSize();
    if(packet->GetProtocol() != IPPROTO_TCP){
        LOG(INFO) << "MPFilter: Protocol is not TCP";
        return 0;
    }
    if(packet->GetL7ContentSize() == 0){
        LOG(INFO) << "MPFilter: ContentSize is 0";
        return 0;
    }
    if(packet->GetStream() == NULL){
        LOG(INFO) << "MPFilter: packet->stream is NULL";
        return 0;
    }

    Stream *stream = packet->GetStream();
    LOG(INFO) << packet->GetL7Content();
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
        LOG(INFO) << "Hrashima PacketNum: " << stream->GetPacketNum();
        LOG(INFO) << "retrieved_content_size: " << retrieved_content_size;
        LOG(INFO) << "content_size: " << content_size;
    }

    int start_flag = 0;
    int offset = 0;
    int depth = 0;
    depth = INT_MAX;
    int start = offset;
    int end = start + depth - 1;
    u_char *p_content = NULL, *p_content_end = NULL;
    int start_place = 0;

    if(start <= retrieved_content_size - 1){ //start packet
            start_flag = 1;
            start_place = start - (retrieved_content_size - content_size);
            p_content = packet->GetL7Content() + start_place;
            if (start_place < 0) {
                LOG(ERROR) << "Calicurate start place";
                LOG(ERROR) << "start place: " << start_place;
                LOG(ERROR) << "retrieved content size: " << retrieved_content_size;
                LOG(ERROR) << "content size: " << content_size;
            }
    }

    if(start_flag == 0){
            start_place = 0;
            p_content = packet->GetL7Content();
    }

    if(end <= retrieved_content_size - 1){ //end packet
        p_content_end = packet->GetL7Content() + end - (retrieved_content_size - content_size);
        if (start_place < 0) {
            LOG(ERROR) << "start place before aho search is called: " << start_place;
        }
        AhoSearch(SUND, start_flag, state, packet, start_place, p_content, p_content_end);
    }else{
        p_content_end = packet->GetL7Content() + content_size - 1;
        if (start_place < 0) {
            LOG(ERROR) << "start place before aho search is called: " << start_place;
        }
        AhoSearch(SUND, start_flag, state, packet, start_place, p_content, p_content_end);
    }

    return 0;
}


MatchPreFilterState::MatchPreFilterState(Stream *stream){
    max_prefilter_pattern_size_ = 0;
    size_t active_rule_size= 0;

    for(list<Rule*>::iterator rule_it = stream->GetRuleFirstIt();
        rule_it != stream->GetRuleLastIt(); rule_it++){

        ActiveRule* temp_rule = new ActiveRule;
        active_rule_size += sizeof(*temp_rule);
        temp_rule->rule_it_ = rule_it;
        temp_rule->rule_state_flag_ = 0;
        temp_rule->kmp_state_ = 0;
        active_rule_list_.push_back(temp_rule);
        int pat_len = ((*rule_it)->GetMatchPreFilterInfo())->pat_len;
        if(max_prefilter_pattern_size_ < pat_len){
            max_prefilter_pattern_size_ = pat_len;
        }
    }
    observer->MPFActiveRuleMallocd(active_rule_size);
    LOG(INFO) << "MPFActivateRuleMalloced";

    after_ip_filter_ = active_rule_list_.size();
    after_pre_filter_ = -1;
    temp_buf_ = new u_char[max_prefilter_pattern_size_ + 1];
    observer->MPFTempBufMallocd(max_prefilter_pattern_size_ * sizeof(unsigned char));

    tmpState_ = 0;
}


MatchPreFilterState::~MatchPreFilterState(){
    for(list<ActiveRule*>::iterator rule_it = active_rule_list_.begin();
        rule_it != active_rule_list_.end(); rule_it++){

        observer->MPFActiveRuleDeleted(sizeof(**rule_it));
        LOG(INFO) << "MPFActiveRuleDeleted: " << sizeof(**rule_it);
        delete *rule_it;
    }
    delete [] temp_buf_;
    observer->MPFTempBufDeleted(max_prefilter_pattern_size_ * sizeof(unsigned char));
    LOG(INFO) << "MPFTempBufDeleted: " << max_prefilter_pattern_size_ * sizeof(unsigned char);
}
