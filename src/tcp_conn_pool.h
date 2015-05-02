/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: TcpConnPool.H,v 5.1 2011-01-17 10:36:40 sin Exp $
*/
#ifndef TCP_CONN_POOL_H_
#define TCP_CONN_POOL_H_
#pragma once

#include "include.h"
#include "tcp_conn.h"
#include "tcp_conn_pool.h"

class TcpConnPool{
 private:
        multimap<unsigned int, TcpConn*> tcp_conn_pool_;

 public:
        TcpConnPool();
        multimap<u_int, TcpConn*>::iterator AddTcpConn(u_int key, TcpConn *tcpconn);

        u_int GetTcpConnCount(u_int key) {return tcp_conn_pool_.count(key);}
        u_int GetSize() {return tcp_conn_pool_.size();}
        multimap<u_int, TcpConn*>::iterator FindTcpConn(u_int key)
         {return tcp_conn_pool_.find(key);}
        multimap<u_int, TcpConn*>::iterator GetTcpConnFirstIt() {return tcp_conn_pool_.begin();}
        multimap<u_int, TcpConn*>::iterator GetTcpConnLastIt() {return tcp_conn_pool_.end();}
        void RemoveTcpConnIt(multimap<u_int, TcpConn*>::iterator it);
        void ShowTcpConnPool();
};
#endif  // TCP_CONN_POOL_H_
