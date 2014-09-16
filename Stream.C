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

#include "Include.H"
#include "Global.H"
#include "Packet.H"
#include "Stream.H"
#include "Rule.H"

PapaResult::PapaResult(){
	result_string[0] = '\0';
	result_size = 0;
	result_offset = 0;
	finished = 0;
}

Stream::Stream(Packet *pkt){
	//MSG("Stream Created")
	//Count stream creation
	observer->StreamCreated();
	observer->StreamMallocd(sizeof(*this));
	OBSERVER_DEBUG(BLUE cout << "StreamMallocd :" << sizeof(*this) << endl;RESET);

	stream_id = stream_id_prev+1;
	stream_id_prev++;

	timestamp = pkt->GetTimestamp();
	src_ip = pkt->GetSrcIP();
	src_port = pkt->GetSrcPort();
	dst_ip = pkt->GetDstIP();
	dst_port = pkt->GetDstPort();
	last_seq_no = pkt->GetSeqNo();
	packet_list.push_back(pkt);
	packet_num = 0;
//	retrieved_content_size = pkt->GetContentSize();
	retrieved_content_size = 0;
	l7_retrieved_content_size = 0;
	state = BEGIN;
	stream_packet_size = 0;
	save_flag = 0;
	tcp_conn = NULL;
	direction = C2S;
	truncated = 0;
	chunk_offset = 0;

	l7_protocol = OTHER;
	http_header_size = 0;
	http_chunked = 0;
	http_content_size = 0;
	http_compress = NONE;
	l7_error = 0;
	disorder_flag = 0;

	gzip_z = NULL;
	gzip_only_http_header = 0;

	//Add stream_pool
	stream_pool_it = stream_pool->AddStream(this);

	//get hole Rule*
	for(list<Rule*>::iterator rule_it = ::rule_pool->GetRuleFirstIt(); rule_it != ::rule_pool->GetRuleLastIt(); rule_it++){
		rule_list.push_back(*rule_it);
	}

	//engine specific process
	match_prefilter_state = 0;
//	Show();

	return;
}

Stream::~Stream(){
	//Count stream deletion
//	MSG("Stream Deleted")
	observer->StreamDeleted();
	observer->StreamFreed(sizeof(*this), stream_packet_size);
	
	if(GetL7Protocol() == HTTP){
		observer->http_stream_num++;
		observer->http_stream_byte_sum += GetRetrievedContentSize();
		observer->http_stream_packet_sum += GetPacketNum();
	}
	
	if(GetHttpCompress()){
		observer->http_gzip_stream_num++;
		observer->http_gzip_stream_byte_sum += GetL7RetrievedContentSize();
		observer->http_gzip_stream_packet_sum += GetPacketNum();
	}

	OBSERVER_DEBUG(BLUE cout << "StreamFreed :" << sizeof(*this) << endl;RESET);

	if(retrieved_content_size > max_stream_size)  max_stream_size = retrieved_content_size;
	//observer->ShowMem(timestamp);

	if(gzip_z != NULL){
		inflateEnd(gzip_z);
		free(gzip_z);
	}

	if(savemode == PACKET){
		//packet was already deleted by each packet recieved time.
		//do nothing.
		//delete(pkt);
	}else if(savemode == STREAM){
		list<Packet *>::iterator it= packet_list.begin();
		while(it != packet_list.end()){
//		for(list<Packet *>::iterator it=packet_list.begin(); it != packet_list.end(); it++){
//			MSG("Steam packets removing!!");
			delete *it;
			it = packet_list.erase((*it)->GetStreamIt());
		}
	}

	//engine specific process
	if(match_prefilter_state != 0){
		observer->MPFStateDeleted(sizeof(*match_prefilter_state));
		OBSERVER_DEBUG(BLUE cout << "MPFStateDeleted :" << sizeof(*match_prefilter_state) << endl;RESET);
		delete match_prefilter_state;
	}
	for(list<PapaResult*>::iterator it = papa_result_list.begin(); it != papa_result_list.end(); it++){
		delete *it;
	}

	stream_pool->RemoveStreamIt(stream_pool_it);
	tcp_conn->RemoveStreamIt(tcp_conn_it);

}

list<Rule*>::iterator Stream::GetRuleFirstIt(){
	return rule_list.begin();
}

list<Rule*>::iterator Stream::GetRuleLastIt(){
	return rule_list.end();
}

list<Rule*>::iterator Stream::RemoveRuleIt(list<Rule*>::iterator it){
	list<Rule*>::iterator it2 = it;
	it2--;
	rule_list.erase(it);
	return it2;
}

Rule* Stream::GetRule(list<Rule*>::iterator it){
	return *it;
}

unsigned int Stream::GetPacketNum(){
	return packet_num;
}

void Stream::Show(){
	YELLOW cout << "STREAM------------------------------------" << endl;RESET
	ShowStreamInfo();
	ShowContents();
}

void Stream::ShowStreamInfo(){
	struct tm * tmp = localtime(&(timestamp.tv_sec));
	YELLOW cout << "Date:"<<tmp->tm_year+1900<<"/"<< tmp->tm_mon+1<<"/"<<tmp->tm_mday<<" "<<tmp->tm_hour<<":"<<tmp->tm_min<<":"<<tmp->tm_sec<<" "<<timestamp.tv_usec <<endl;
	cout << "IP: " << src_ip_str << ":" << src_port << " -> " << dst_ip_str << ":" << dst_port <<endl;
	RESET
}

void Stream::ShowContents(){
	YELLOW cout << "Contents: ["; RESET
	for(list<Packet*>::iterator it=++packet_list.begin(); it!=packet_list.end(); it++){
		(*it)->ShowContent();
	}
	YELLOW cout << "]"<<endl; RESET
}

void Stream::ShowPackets(){
	for(list<Packet*>::iterator it=++packet_list.begin(); it!=packet_list.end(); it++){
		(*it)->Show();
	}
}

void Stream::AddRule(Rule *rule){
	rule_list.push_back(rule);
	return;
}

void Stream::AddPacket(Packet *pkt){
	if(pkt->GetSeqNo() != last_seq_no + GetLastPacketContentSize() && GetState() == CONTINUE){
		if(pkt->GetSeqNo() <= last_seq_no){
			//duplicate! do nothing
			pkt->SetError();
			return;
		}else{
			//out of order
			pkt->SetError();
			if(disorder_flag != 1){
	//			cout << "disorder!!" << endl;
	//			cout << "last:"<< last_seq_no << " now: " << pkt->GetSeqNo() << endl;
				disorder_flag = 1;
				observer->StreamDisorderd();
			}
			return;
		}
	}

	if(disorder_flag == 1){
		//diosrder recoverd by retransmit
	//	cout << "disorder recoverd!!" << endl;
		observer->StreamDisorderRecoverd();
		disorder_flag = 0;
	}

	last_seq_no = pkt->GetSeqNo();
//	packet_list.push_back(pkt);
	pkt->SetStreamIt(packet_list.insert(packet_list.end(),pkt));
	pkt->SetStream(this);
	packet_num++;
//	if(retrieved_content_size == 0){
//		cout << "Stream.C packet_num:" <<  packet_num << " retrieved_content_size: " << retrieved_content_size << " content_size: " << pkt->GetContentSize() << endl;
//	}
	retrieved_content_size += pkt->GetContentSize();
	l7_retrieved_content_size += pkt->GetContentSize();
	last_updated_time = pkt->GetTimestamp();
	stream_packet_size += pkt->GetPacketSize();
	last_packet_content_size = pkt->GetContentSize();

	observer->SimPacketMallocd(pkt->GetPacketSize());
	return;
}

void Stream::RemovePacketIt(list<Packet *>::iterator it){
	packet_list.erase(it);
//	packet_num--;
}

MatchPreFilterState* Stream::GetMatchPreFilterState(){
	return match_prefilter_state;
}

void Stream::SetMatchPreFilterState(MatchPreFilterState *state){
	match_prefilter_state = state;
	return;
}

string Stream::GetRuleIds(){
	string res;
	for(list<Rule*>::iterator it=rule_list.begin(); it != rule_list.end(); it++){
		//res += "["+ lexical_cast<string>((*it)->GetId()) +"]";
		sprintf(buffer, "[%d]", (*it)->GetId());
		res += buffer;
	}
	return res;
}

/*
void Stream::GabageDelete(){
	gc_deleted = 1;
	observer->StreamDeleted();
	observer->StreamFreed(sizeof(*this), stream_packet_size);
	};
	*/
/*
void Stream::Reconstruct(){
	stream = (unsigned char *)malloc(retrieved_content_size);
	unsigned char *p_st;
	for(list<Packet *>::iterator it=packet_list.begin(); it != packet_list.end(); it++){
		memcpy(p_st, (*it)->GetContent(), (*it)->GetContentSize());
		p_st += (*it)->GetContentSize();
	}
}
*/
