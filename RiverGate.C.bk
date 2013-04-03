/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: RiverGate.C,v 5.6 2012-05-19 15:01:31 sin Exp $
*/

#include "Global.H"
#include "Include.H"
#include "RiverGate.H"
#include "Stream.H"

RiverGate::RiverGate(){
	connectCli(RIVERPORT,RIVERHOST);
	return;
}
RiverGate::~RiverGate(){
  if (close(sockfd)) exit(1);
}

int RiverGate::connectCli(const int port, const char hostname[]) {
  struct hostent *ent;
  struct sockaddr_in addr;

  bzero((char *)&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  if ((ent = gethostbyname(hostname)) == NULL) cerr << "RiverGate:hostname error" <<endl;
  bcopy(ent->h_addr, (char *)&addr.sin_addr, ent->h_length);
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) cerr << "RiverGate: Socket error" << endl ;
  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) cerr << "RiverGate: connect error" << endl;

  return sockfd;
}



void RiverGate::ProcStream(Stream *stream){
	stream->GetTimestamp();
	stream->GetSrcIP();
	stream->GetDstIP();
	
	stream->GetStreamId();
	stream->GetSrcPort();
	stream->GetDstPort();
	stream->GetDirection();
	stream->GetTruncated();
	stream->GetStreamSize();
	stream->GetOrgStreamSize();
	stream->GetError();
	stream->GetL7Error();
	stream->GetL7Protocol();
	stream->GetHttpContentSize();
	stream->GetHttpCompress();
	stream->GetHttpHeaderSize();
	stream->GetHttpChunked();
	stream->GetHitCount(); 

	return;
}

void RiverGate::ProcPacket(Packet *pkt){

	pkt->GetTimestamp();
	pkt->GetSrcIP();
	pkt->GetDstIP();
	pkt->GetSrcPort();
	pkt->GetDstPort();
	pkt->GetProtocol();
	pkt->GetPacketSize();

	if(pkt->GetProtocol() == IPPROTO_TCP){
		if(pkt->GetAck()){ }
		if(pkt->GetFin()){ }
		if(pkt->GetSyn()){ }
		if(pkt->GetRst()){ }
	}else{
		return;
	}

	pkt->GetContent();
	pkt->GetContentSize();

	pkt->GetL7Content();
	pkt->GetL7ContentSize();


	int ack = 0;

	char* send_buf = (char*)malloc(sizeof(char)*BUFSIZ);
	bzero(send_buf, sizeof(char)*BUFSIZ);
  /* Send Schema Info */
  strcpy(send_buf, "pkt1");
  if (send(sockfd, send_buf, BUFSIZ, 0) == -1) exit(1);    
  if (recv(sockfd, &ack, sizeof(ack), MSG_WAITALL) == -1) exit(1);    
	bzero(send_buf, sizeof(char)*BUFSIZ);

  /* Send tuples */
	char* str = (char*)malloc(sizeof(char)*BUFSIZ);
	bzero(str, sizeof(char)*BUFSIZ);
	int c = 0;

    //2009-12-14 14:35:51, 209.85.222.7, 130.158.76.6, 36061, 110, 1, 15
	
	//date
//    str = ctime(pkt->GetTimestamp().tv_sec); strcpy(send_buf + c, buf); c += SZTXT;
//	cout << str;
	struct tm *tmp = localtime(&((pkt->GetTimestamp()).tv_sec));
	ostringstream oss;
	oss << tmp->tm_year+1900 <<"-"<< tmp->tm_mon+1 <<"-"<<tmp->tm_mday <<" "<<tmp->tm_hour<<":"<<tmp->tm_min<<":"<<tmp->tm_sec;
	string tstamp = oss.str();
//    strcpy(send_buf + c, tstamp.c_str()); c += SZTXT;
//	cout << tstamp << "	";


	//src ip
	str = inet_ntoa(pkt->GetSrcIP()); strcpy(send_buf + c, str); c += sizeof(int);
	cout << str << "	";

	//dst ip
	str = inet_ntoa(pkt->GetDstIP()); strcpy(send_buf + c, str); c += sizeof(int);
	cout << str << "	";

	//src port
	int hoge = pkt->GetSrcPort();
	memcpy(send_buf + c, &hoge,sizeof(int)); c += sizeof(int);
	cout << hoge << "	";

	//dst port
	hoge = pkt->GetDstPort();
	memcpy(send_buf + c, &hoge,sizeof(int)); c += sizeof(int);
	cout << hoge << "	";

	//direction
	hoge = 1;
	memcpy(send_buf + c, &hoge,sizeof(int)); c += sizeof(int);
	cout << hoge << "	";

	//content size
	hoge = 12;
	memcpy(send_buf + c, &hoge,sizeof(int)); c += sizeof(int);
	cout << hoge << "	";

	cout << endl;
	cout << endl;
//	cout << "START" << endl;
//	cout << send_buf << endl;
//	cout << "END" << endl;
 //   cout << endl;
//	cout << "size c=" << c << endl;

    if (send(sockfd, send_buf, c, 0) == -1) exit(1);    
	if (recv(sockfd, &ack, sizeof(ack), MSG_WAITALL) == -1) exit(1);    
 //   if (send(sockfd, "this is test", sizeof("this is test"), 0) == -1) exit(1);    
// 	static int count = 0;
//	if(count >= 10){
//		exit(0);
//	}


	return;
}

void RiverGate::ProcResult(u_int stream_id, u_int rule_id, u_char *result_string, u_int result_size){
//	stream_id;
//	rule_id;
//	*result_string;
//	result_size;
}
	

