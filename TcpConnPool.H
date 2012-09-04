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

#pragma once

#include "Include.H"
#include "TcpConn.H"
#include "TcpConnPool.H"

class TcpConnPool
{
	private:
		multimap<unsigned int, TcpConn*> tcp_conn_pool;

	public:
		TcpConnPool();
		multimap<u_int, TcpConn*>::iterator AddTcpConn(u_int key, TcpConn *tcpconn);
		
		u_int GetTcpConnCount(u_int key){return tcp_conn_pool.count(key);}
		u_int GetSize(){return tcp_conn_pool.size();}
		multimap<u_int, TcpConn*>::iterator FindTcpConn(u_int key){return tcp_conn_pool.find(key);}
		multimap<u_int, TcpConn*>::iterator GetTcpConnFirstIt(){return tcp_conn_pool.begin();}
		multimap<u_int, TcpConn*>::iterator GetTcpConnLastIt(){return tcp_conn_pool.end();}
		void RemoveTcpConnIt(multimap<u_int, TcpConn*>::iterator it); 
		void ShowTcpConnPool();
};
