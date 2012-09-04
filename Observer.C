/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Observer.C,v 5.11 2012-05-01 09:14:49 sin Exp $
*/
#include "Observer.H"
#include "Global.H"

Observer::Observer(){
	SetDefault();
	//Print HTTP decoding info
	//cout << "time(real)"<< "	" << "time(packet)" << "	" << "	" << "all_stream_byte_sum" << "	" << "all_stream_packet_sum"  <<"	" <<  "all_stream_num" << "	" << "	" << "http_gzip_stream_byte_sum" << "	" << "http_gzip_stream_packet_sum"  <<"	" <<  "http_gzip_stream_num" << endl;
	cout << "time(real)"<< "	" << "time(packet)" << "	" << "	" << "disorder_count" <<"	" << "stream_created_num" << "	"<< "stream_current_num" << endl;
}

Observer::~Observer(){
	return;
}

void Observer::SetDefault(){

		num_packet_created = 0;
		num_packet_deleted = 0;
		num_stream_created = 0;
		num_stream_deleted = 0;
		num_tcpconn_created = 0;
		num_tcpconn_deleted = 0;
		max_stream_size = 0;
//		mpf_mem_size = 544+16+48+160; //ActiveRules + TempBuf + State + Stream
	//	mpf_mem_size = 272+13+48+160; //ActiveRules + TempBuf + State + Stream
		mpf_mem_size = 48+ 16*sim_rule_num + sim_rule_size; //ActiveRules + TempBuf + State + Stream
//	cout << "mpf sim_rule_num: " << sim_rule_num << endl;
//	cout << "mpf sim_rule_size: " << sim_rule_size << endl;
//	cout << "mpf mem size: " << mpf_mem_size << endl;

	return;
}

void Observer::Show(){
	cout << "----Observer result------------------" << endl;
	cout << "Packet created: " << num_packet_created << endl;
	cout << "Packet deleted: " << num_packet_deleted << endl;
	cout << "Packet memory mallocd(byte): " << num_packet_mallocd << endl;
	cout << "Packet memory freed(byte)  : " << num_packet_freed << endl;
	cout << "Packet memory INUSE  : " << num_packet_mem_inuse << endl;
	cout <<endl;
	cout << "Stream created: " << num_stream_created << endl;
	cout << "Stream deleted: " << num_stream_deleted << endl;
	cout << "Stream memory mallocd(byte): " << stream_mem_mallocd << endl;
	cout << "Stream memory freed(byte)  : " << stream_mem_freed << endl;
	cout << "Stream memory INUSE  : " << stream_mem_inuse << endl;

	cout <<endl;
	cout << "TcpConn created: " << num_tcpconn_created << endl;
	cout << "TcpConn deleted: " << num_tcpconn_deleted << endl;
	cout <<endl;
//	cout << "MPFActiveRule mallocd: " << mpf_activerule_mallocd << endl;
//	cout << "MPFActiveRule freed:   " << mpf_activerule_freed << endl;
//	cout << "MPFActiveRule inuse:   " << mpf_activerule_mem_inuse << endl;
//	cout << "MPFTempbuf mallocd: " << mpf_tempbuf_mallocd << endl;
//	cout << "MPFTempbuf freed:   " << mpf_tempbuf_freed << endl;
//	cout << "MPFTempbuf inuse:   " << mpf_tempbuf_mem_inuse << endl;
//	cout << "MPFState mallocd: " << mpf_state_mallocd << endl;
//	cout << "MPFState freed:   " << mpf_state_freed << endl;
//	cout << "MPFState inuse:   " << mpf_state_mem_inuse << endl;
	cout << "----Observer END------------------" << endl;
}

void Observer::ShowMem(timeval timestamp){
	if(offset_time.tv_sec == 0 && offset_time.tv_usec == 0) offset_time = timestamp;

	unsigned long int stream_num = 0;
	unsigned long int sum_stream_size = 0;
	unsigned long int gzip_stream_num = 0;

	for(list<Stream *>::iterator it = stream_pool->GetStreamFirstIt(); it != stream_pool->GetStreamLastIt(); it++){
		stream_num++;
		sum_stream_size += (*it)->GetRetrievedContentSize();
		if((*it)->GetRetrievedContentSize() >= max_stream_size){
			max_stream_size =(*it)->GetRetrievedContentSize();
		}
		if((int)((*it)->GetHttpCompress())){
			gzip_stream_num++;
		}
	}
	if(stream_num * 5 + gzip_stream_num * 64 > max_stream_num) max_stream_num = stream_num*5+gzip_stream_num*64;
	if(sum_stream_size > max_sum_stream_size) max_sum_stream_size = sum_stream_size;
/*
	cout << "timestamp:	" << timestamp.tv_sec - offset_time.tv_sec << " stream_num:	" << stream_num \
		<< " max_stream_size:	"<< max_stream_size << " sum_stream_size:	" << sum_stream_size << \
		 " gzip_stream_num:	" << gzip_stream_num << endl;

	cout << timestamp.tv_sec - offset_time.tv_sec << "	" << stream_num \
		<< "	"<< max_stream_size << "	" << sum_stream_size << \
		 "	" << gzip_stream_num << endl;
*/

	//cout << timestamp.tv_sec - offset_time.tv_sec << "	" << num_packet_mem_inuse + stream_mem_inuse + mpf_mem_inuse << "	" << sim_stream_mode_mem_inuse<< "	" << mpf_mem_inuse << endl;

	//Memory size comparison : SRIM vs TRADITIONAL
	//cout << timestamp.tv_sec - offset_time.tv_sec << "	" << num_packet_mem_inuse + stream_mem_inuse + mpf_mem_inuse << "	" << sim_stream_mode_mem_inuse << endl;
	
	//Print Queue Size
	//cout << timestamp.tv_sec - offset_time.tv_sec << "	" << std::setw(15) << packet_queue->GetQueueSize() << endl;
	
	//Print HTTP decoding info
	//cout << timestamp.tv_sec - offset_time.tv_sec << "	" << std::setw(15) << http_stream_byte_sum << std::setw(15) << http_stream_packet_sum  <<std::setw(15) <<  http_stream_num << "	" << std::setw(15) << http_gzip_stream_byte_sum << std::setw(15) << http_gzip_stream_packet_sum  <<std::setw(15) <<  http_gzip_stream_num << endl;
	cout << timestamp.tv_sec - offset_time.tv_sec << "	" << std::setw(15) << num_stream_disorderd << "	" << std::setw(15) << num_stream_created << "	" << std::setw(15) << num_stream_created - num_stream_deleted  << endl;

}

void Observer::PacketCreated(){
	num_packet_created++;
}

void Observer::PacketDeleted(){
	num_packet_deleted++;
}

void Observer::StreamCreated(){
	num_stream_created++;
	mpf_mem_inuse += mpf_mem_size;
}

void Observer::StreamDeleted(){
	num_stream_deleted++;
	mpf_mem_inuse -= mpf_mem_size;
}

void Observer::StreamDisorderd(){
	num_stream_disorderd++;
}

void Observer::StreamDisorderRecoverd(){
	num_stream_disorderd--;
}

void Observer::TcpConnCreated(){
	num_tcpconn_created++;
}

void Observer::TcpConnDeleted(){
	num_tcpconn_deleted++;
}
	
void Observer::PacketMallocd(size_t size){
	num_packet_mallocd += size;
	num_packet_mem_inuse += size;
}

void Observer::PacketFreed(size_t size){
	num_packet_freed += size;
	num_packet_mem_inuse -= size;
}

void Observer::StreamMallocd(size_t size){
	stream_mem_mallocd += size;
	stream_mem_inuse += size;
}

void Observer::SimPacketMallocd(size_t size){
	sim_stream_mode_mem_inuse += size;
}

void Observer::StreamFreed(size_t size, size_t streampktsize){
	stream_mem_freed += size;
	stream_mem_inuse -= size;
	
	sim_stream_mode_mem_inuse -= streampktsize;

}

void Observer::MPFActiveRuleMallocd(size_t size){
	mpf_activerule_mallocd += size;
	mpf_activerule_mem_inuse += size;
}

void Observer::MPFActiveRuleDeleted(size_t size){
	mpf_activerule_freed += size;
	mpf_activerule_mem_inuse -= size;
}

void Observer::MPFTempBufMallocd(size_t size){
	mpf_tempbuf_mallocd += size;
	mpf_tempbuf_mem_inuse += size;
}
	
void Observer::MPFTempBufDeleted(size_t size){
	mpf_tempbuf_freed += size;
	mpf_tempbuf_mem_inuse -= size;
}
void Observer::MPFStateMallocd(size_t size){mpf_state_mallocd += size; mpf_state_mem_inuse += size;};
void Observer::MPFStateDeleted(size_t size){mpf_state_freed += size; mpf_state_mem_inuse -= size;};
