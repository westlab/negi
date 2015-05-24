/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Stream.C,v 5.23 2012/08/23 04:04:12 sin Exp $
*/

#include "include.h"
#include "global.h"
#include "packet.h"
#include "stream.h"
#include "rule.h"
#include "glog/logging.h"

PapaResult::PapaResult() {
    result_string_[0] = '\0';
    result_size_ = 0;
    result_offset_ = 0;
    finished_ = 0;
}

Stream::Stream(Packet *pkt) {
    LOG(INFO) << "Stream Created";
    // MSG("Stream Created")
    // Count stream creation
    observer->StreamCreated();
    observer->StreamMallocd(sizeof(*this));
    OBSERVER_DEBUG(BLUE cout << "StreamMallocd :" << sizeof(*this) << endl; RESET);

    stream_id_ = stream_id_prev+1;
    stream_id_prev++;

    timestamp_ = pkt->GetTimestamp();
    src_ip_ = pkt->GetSrcIP();
    src_port_ = pkt->GetSrcPort();
    dst_ip_ = pkt->GetDstIP();
    dst_port_ = pkt->GetDstPort();
    last_seq_no_ = pkt->GetSeqNo();
    packet_list_.push_back(pkt);
    packet_num_ = 0;
    retrieved_content_size_ = 0;
    l7_retrieved_content_size_ = 0;
    state_ = BEGIN;
    stream_packet_size_ = 0;
    save_flag_ = 0;
    tcp_conn_ = NULL;
    direction_ = C2S;
    truncated_ = 0;
    chunk_offset_ = 0;

    l7_protocol_ = OTHER;
    http_header_size_ = 0;
    http_chunked_ = 0;
    http_content_size_ = 0;
    http_compress_ = NONE;
    l7_error_ = 0;
    disorder_flag_ = 0;

    gzip_z_ = NULL;
    gzip_only_http_header_ = 0;

    // Add stream_pool
    stream_pool_it_ = stream_pool->AddStream(this);

    // get hole Rule*
    for (list<Rule*>::iterator rule_it = ::rule_pool->GetRuleFirstIt();
        rule_it != ::rule_pool->GetRuleLastIt(); rule_it++) {
        rule_list_.push_back(*rule_it);
    }

    // engine specific process
    match_prefilter_state_ = NULL;
    // Show();

    return;
}

Stream::~Stream() {
    // Count stream deletion
    // MSG("Stream Deleted")
    observer->StreamDeleted();
    observer->StreamFreed(sizeof(*this), stream_packet_size_);

    if (GetL7Protocol() == HTTP) {
        observer->http_stream_num++;
        observer->http_stream_byte_sum += GetRetrievedContentSize();
        observer->http_stream_packet_sum += GetPacketNum();
    }

    if (GetHttpCompress()) {
        observer->http_gzip_stream_num++;
        observer->http_gzip_stream_byte_sum += GetL7RetrievedContentSize();
        observer->http_gzip_stream_packet_sum += GetPacketNum();
    }

    OBSERVER_DEBUG(BLUE cout << "StreamFreed :" << sizeof(*this) << endl; RESET);

    if (retrieved_content_size_ > max_stream_size)  max_stream_size = retrieved_content_size_;
    // observer->ShowMem(timestamp);

    if (gzip_z_ != NULL) {
        inflateEnd(gzip_z_);
        free(gzip_z_);
    }

    if (savemode == PACKET) {
        // packet was already deleted by each packet recieved time.
        // do nothing.
        // delete(pkt);
    } else if (savemode == STREAM) {
        list<Packet *>::iterator it = packet_list_.begin();
        while (it != packet_list_.end()) {
            delete *it;
            it = packet_list_.erase((*it)->GetStreamIt());
        }
    }

    // engine specific process
    if (match_prefilter_state_ != 0) {
        observer->MPFStateDeleted(sizeof(*match_prefilter_state_));
        OBSERVER_DEBUG(BLUE cout << "MPFStateDeleted :"
        << sizeof(*match_prefilter_state) << endl; RESET);
        delete match_prefilter_state_;
    }
    for (list<PapaResult*>::iterator it = papa_result_list_.begin();
         it != papa_result_list_.end(); it++) {
        delete *it;
    }

    stream_pool->RemoveStreamIt(stream_pool_it_);
    tcp_conn_->RemoveStreamIt(tcp_conn_it_);
}

list<Rule*>::iterator Stream::GetRuleFirstIt() {
    return rule_list_.begin();
}

list<Rule*>::iterator Stream::GetRuleLastIt() {
    return rule_list_.end();
}

list<Rule*>::iterator Stream::RemoveRuleIt(list<Rule*>::iterator it) {
    list<Rule*>::iterator it2 = it;
    it2--;
    rule_list_.erase(it);
    return it2;
}

Rule* Stream::GetRule(list<Rule*>::iterator it) {
    return *it;
}

unsigned int Stream::GetPacketNum() {
    return packet_num_;
}

void Stream::Show() {
    YELLOW cout << "STREAM------------------------------------" << endl; RESET
    ShowStreamInfo();
    ShowContents();
}

void Stream::ShowStreamInfo() {
    struct tm local_tm;
    struct tm * tmp = localtime_r(&(timestamp_.tv_sec), &local_tm);
    YELLOW cout << "Date:" << tmp->tm_year+1900 << "/"
    << tmp->tm_mon+1 << "/" << tmp->tm_mday << " " << tmp->tm_hour << ":"
    << tmp->tm_min << ":" << tmp->tm_sec << " " << timestamp_.tv_usec << endl;
    cout << "IP: " << src_ip_str_ << ":" << src_port_ << " -> "
    << dst_ip_str_ << ":" << dst_port_ <<endl;
    RESET
}

void Stream::ShowContents() {
    YELLOW cout << "Contents: ["; RESET
    for (list<Packet*>::iterator it=++packet_list_.begin(); it != packet_list_.end(); it++) {
        (*it)->ShowContent();
    }
    YELLOW cout << "]" << endl; RESET
}

void Stream::ShowPackets() {
    for (list<Packet*>::iterator it=++packet_list_.begin(); it != packet_list_.end(); it++) {
        (*it)->Show();
    }
}

void Stream::AddRule(Rule *rule) {
    rule_list_.push_back(rule);
    return;
}

void Stream::AddPacket(Packet *pkt) {
    if (pkt->GetSeqNo() != last_seq_no_ + GetLastPacketContentSize() && GetState() == CONTINUE) {
        if (pkt->GetSeqNo() <= last_seq_no_) {
            // duplicate! do nothing
            pkt->SetError();
            return;
        } else {
            // out of order
            pkt->SetError();
            if (disorder_flag_ != 1) {
                disorder_flag_ = 1;
                observer->StreamDisorderd();
            }
            return;
        }
    }

    if (disorder_flag_ == 1) {
        observer->StreamDisorderRecoverd();
        disorder_flag_ = 0;
    }

    last_seq_no_ = pkt->GetSeqNo();
    pkt->SetStreamIt(packet_list_.insert(packet_list_.end(), pkt));
    pkt->SetStream(this);
    packet_num_++;
    retrieved_content_size_ += pkt->GetContentSize();
    l7_retrieved_content_size_ += pkt->GetContentSize();
    last_updated_time_ = pkt->GetTimestamp();
    stream_packet_size_ += pkt->GetPacketSize();
    last_packet_content_size_ = pkt->GetContentSize();

    observer->SimPacketMallocd(pkt->GetPacketSize());
    return;
}

void Stream::RemovePacketIt(list<Packet *>::iterator it) {
    packet_list_.erase(it);
}

MatchPreFilterState* Stream::GetMatchPreFilterState() {
    return match_prefilter_state_;
}

void Stream::SetMatchPreFilterState(MatchPreFilterState *state) {
    match_prefilter_state_ = state;
    return;
}

string Stream::GetRuleIds() {
    string res;
    for (list<Rule*>::iterator it=rule_list_.begin(); it != rule_list_.end(); it++) {
        // res += "["+ lexical_cast<string>((*it)->GetId()) +"]";
        sprintf(buffer_, "[%d]", (*it)->GetId());
        res += buffer_;
    }
    return res;
}
