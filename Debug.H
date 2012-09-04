/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Debug.H,v 5.9 2011-11-16 10:45:44 sin Exp $
*/
#pragma once

//#define CP(in) cout << "[" << in << "]" << flush;
#define MSG(in) cout << "[" << in << "]" << endl<< flush;
#define MSGH(x)\
	do { \
		cout.setf( ios::hex, ios::basefield ); \
		cout << "[" << x << "]" << endl; \
		cout.setf( ios::dec, ios::basefield ); \
	} while (0);
//#define CP(in)

//#define DEBUG_BEGIN(level) if(config[debuglevel] <= level){ YELLOW
#define DEBUG_BEGIN(level) if(level <= config["debuglevel"]){ YELLOW
#define DEBUG_END cout << flush; RESET }

//#define ST_REBUILD_DEBUG_EN
//#define PACKET_DEBUG_EN
//#define MASTER_DEBUG_EN
//#define MPF_DEBUG_EN
//#define OBSERVER_DEBUG_EN
//#define ERROR_DEBUG_EN
//#define HTTP_DEBUG_EN

#define HOGE cout << "hoge" << endl;

#ifdef ERROR_DEBUG_EN
#define ERROR_DEBUG(x)  x ;
#else
#define ERROR_DEBUG(x)   ;
#endif


//#define STREAM_CLEAN_MODE
