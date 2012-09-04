/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Test.H,v 5.0 2010-10-13 08:47:17 sin Exp $
*/
#pragma once

class Test{
	public:
		Test();
		u_int CreateDummyPacket(u_char *packet, string src_ip, string dst_ip, u_int src_port, u_int dst_port, u_char* cont, u_int cont_size);
};
