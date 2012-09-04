/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: TcpConnPool.C,v 5.0 2010-10-13 08:47:17 sin Exp $
*/

#include "TcpConnPool.H"

TcpConnPool::TcpConnPool(){
	return;
}

multimap<u_int, TcpConn*>::iterator TcpConnPool::AddTcpConn(u_int key, TcpConn *tcpconn){
	return tcp_conn_pool.insert(multimap<u_int, TcpConn*>::value_type(key, tcpconn));
}


void TcpConnPool::RemoveTcpConnIt(multimap<u_int, TcpConn*>::iterator it){
	tcp_conn_pool.erase(it);
	return ;
}

void TcpConnPool::ShowTcpConnPool(){
	for(multimap<u_int, TcpConn*>::iterator it = tcp_conn_pool.begin(); it!=tcp_conn_pool.end(); it++){
		(it->second)->Show();
	}
}
