/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Define.H,v 5.19 2012/05/23 05:23:48 sin Exp $
*/


#pragma once

#define SAVE_MAX_SIZE 102400 	//100KB
#define RESULT_SIZE 200
//#define MAX_QUEUE_SIZE 625000	//1GB
//#define MAX_QUEUE_SIZE 6250	//10MB
#define MAX_QUEUE_SIZE 100	//1GB
#define BUFF_SIZE	1000

#define P_MAX 4

//for MatchPreFilter
#define STRING_MATCH_NUM 3

//#define USE_POSTGRES
#define USE_SQLITE
#define SQLITE_MODE
#define POSTGRES_MODE
//#define FILEWRITE_MODE
#define DEBUG

enum Trunc{
	FULL, TRUNCATED
};

enum L7Protocol{
	OTHER = 0,	HTTP = 80,
};
enum HttpCompress{
	NONE, DEFLATE, GZIP
};

enum State{
	BEGIN, CONTINUE, END, DISCARD, REMOVE
};

enum SaveMode{
	PACKET,STREAM
};

enum MallocMode{
	MALLOC,SIMULATE
};

enum StringMatch{
//    KMP, BM1, BM2, BM3, HORS, SUND, PAPA1, PAPA2
	BM2, HORS, SUND
};

enum ProcList{
	MANAGER, MASTER, CAPTURE
};


