/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: TcpConn.C,v 5.9 2011-11-04 03:22:11 sin Exp $
*/

#include "Include.H"
#include "Global.H"
#include "TcpConn.H"

TcpConn::TcpConn(unsigned int port_map_key, Packet * pkt){
	//Count tcpconn cleation
	observer->TcpConnCreated();
	timestamp = pkt->GetTimestamp();
	last_updated_time = pkt->GetTimestamp();
	src_ip = pkt->GetSrcIP();
	src_port = pkt->GetSrcPort();
	dst_ip = pkt->GetDstIP();
	dst_port = pkt->GetDstPort();

	prev_direction = NEW;
	destructing_flag = 0;

	tcp_conn_pool_it = tcp_conn_pool->AddTcpConn(port_map_key, this);
}

TcpConn::~TcpConn(){
	//Count tcpconn deletion
	observer->TcpConnDeleted();

	destructing_flag=1;

	tcp_conn_pool->RemoveTcpConnIt(tcp_conn_pool_it);

//	cout << "TcpConn Destructor!! stream num: " << stream_list.size() << endl;

	if(stream_list.size() !=0){
		list<Stream *>::iterator it= stream_list.begin();
		list<Stream *>::iterator it2;
		while(it != stream_list.end()){
//			if(it == stream_list.begin()){
//				it2 = it;
//			}else{
				it2 = --it;
				++it;
//			}
//			MSG("Steam removing!!");
//			cout << "Stream ID is:" <<(*it)->GetStreamId() << endl;
			if(it != stream_list.end()){
				delete *it;
			}
			it = it2;
		}
	}
//	cout << "TcpConn Destructed"<< endl;

}

void TcpConn::AddStream( Stream *stream){
	last_updated_time = stream->GetTimestamp();
	stream->SetTcpConnIt(stream_list.insert(stream_list.end(),stream));
//	stream_list.push_back(stream);
//	stream->SetTcpConnIt(--stream_list.end());
	stream->SetTcpConn(this);
}

void TcpConn::SetPrevDirection(u_char dir){
	prev_direction = dir;
}

list<Stream*>::iterator TcpConn::GetStreamFirstIt(){
	return stream_list.begin();
};
list<Stream*>::iterator TcpConn::GetStreamLastIt(){
	return stream_list.end();
};
list<Stream*>::iterator TcpConn::RemoveStreamIt(list<Stream*>::iterator it){
	//cout << "Erace stream_list entry. Stream ID is:" <<(*it)->GetStreamId() << endl;
	stream_list.erase(it);
	if(stream_list.size() == 0 && destructing_flag == 0){
		//cout << "deleting myself!" << endl;
		delete this;
	}
	return it;
};

void TcpConn::Show(){
	for(list<Stream*>::iterator it = stream_list.begin(); it != stream_list.end(); it++){
		(*it)->Show();
	}
}

