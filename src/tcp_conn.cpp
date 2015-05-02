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

#include "include.h"
#include "global.h"
#include "tcp_conn.h"

TcpConn::TcpConn(unsigned int port_map_key, Packet * pkt) {
    // Count tcpconn cleation
    observer->TcpConnCreated();
    timestamp_ = pkt->GetTimestamp();
    last_updated_time_ = pkt->GetTimestamp();
    src_ip_ = pkt->GetSrcIP();
    src_port_ = pkt->GetSrcPort();
    dst_ip_ = pkt->GetDstIP();
    dst_port_ = pkt->GetDstPort();

    prev_direction_ = NEW;
    destructing_flag_ = 0;

    tcp_conn_pool_it_ = tcp_conn_pool->AddTcpConn(port_map_key, this);
}

TcpConn::~TcpConn() {
    // Count tcpconn deletion
    observer->TcpConnDeleted();
    destructing_flag_ = 1;

    tcp_conn_pool->RemoveTcpConnIt(tcp_conn_pool_it_);
    if (stream_list_.size() !=0) {
        list<Stream *>::iterator it = stream_list_.begin();
        list<Stream *>::iterator it2;
        while (it != stream_list_.end()) {
                it2 = --it;
                ++it;
            if (it != stream_list_.end()) {
                delete *it;
            }
            it = it2;
        }
    }
}

void TcpConn::AddStream(Stream *stream) {
    last_updated_time_ = stream->GetTimestamp();
    stream->SetTcpConnIt(stream_list_.insert(stream_list_.end(), stream));
    stream->SetTcpConn(this);
}

void TcpConn::SetPrevDirection(u_char dir) {
    prev_direction_ = dir;
}

list<Stream*>::iterator TcpConn::GetStreamFirstIt() {
    return stream_list_.begin();
}

list<Stream*>::iterator TcpConn::GetStreamLastIt() {
    return stream_list_.end();
}

list<Stream*>::iterator TcpConn::RemoveStreamIt(list<Stream*>::iterator it) {
    stream_list_.erase(it);
    if (stream_list_.size() == 0 && destructing_flag_ == 0) {
        // cout << "deleting myself!" << endl;
        delete this;
    }
    return it;
}

void TcpConn::Show() {
    for (list<Stream*>::iterator it = stream_list_.begin(); it != stream_list_.end(); it++) {
        (*it)->Show();
    }
}
