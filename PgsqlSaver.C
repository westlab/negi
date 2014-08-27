/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: PgsqlSaver.C,v 5.11 2012-05-09 17:24:13 sin Exp $
*/

#include "Include.H"
#include "Global.H"
#include "Stream.H"

#ifdef USE_POSTGRES

#include "PgsqlSaver.H"

PgsqlSaver::PgsqlSaver(){
	return;
}


void PgsqlSaver::Proc(Stream * stream){

	//add
	struct timeval tmp_time = stream->GetTimestamp();
	struct tm *tmp = localtime(&tmp_time.tv_sec);
	ostringstream oss;
	oss << tmp->tm_year+1900 <<"-"<< tmp->tm_mon+1 <<"-"<<tmp->tm_mday <<" "<<tmp->tm_hour<<":"<<tmp->tm_min<<":"<<tmp->tm_sec;
	string tstamp = oss.str();

//	cout << str_src_addr <<":"<<str_dst_addr << endl;

	oss.str("");
	oss << "insert into save_stream ("
	<<"id, srim_version, src_ip, dst_ip, src_port, dst_port, timestamp, "
	<<"direction ,truncate , regexp_id, "
	<<"st_size, st_size_org, "
	<<"error, "
	<<"l7_error, "
	<<"l7prot, http_size, http_compress, "
	<<"http_header_size, http_chunked, hit_count, "

	<<"match_str, stream, stream_org, "
	<<"after_ipfilter, after_prefilter, prefilter_log"
	<<") values ('"
	<< stream->GetStreamId() << "','2.2','"<<stream->GetSrcIPStr()<<"','"<<stream->GetDstIPStr()<<"','"<<stream->GetSrcPort()<<"','"<<stream->GetDstPort()<<"','"<<tstamp<<"','"
	<< stream->GetDirection() <<"','"<< stream->GetTruncated() <<"','"<< stream->GetRuleIds() <<"','"
//	<< stream->GetDirection() <<"','"<< stream->GetTruncated() <<"','""','"
	<< stream->GetStreamSize() <<"','"<<  stream->GetOrgStreamSize() <<"','"
	<< stream->GetError() <<"','"
	<< stream->GetL7Error() <<"','"
	<< stream->GetL7Protocol() <<"','"<<  stream->GetHttpContentSize() <<"','"<<  stream->GetHttpCompress() <<"','" \
	<< stream->GetHttpHeaderSize() <<"','"<<  stream->GetHttpChunked() <<"','"<<  stream->GetHitCount();
	string query = oss.str();


	//match_str
	//query += "',E'"+T.esc_raw(stream->GetMatchString())+"'";
	query += "',E'"+escape_binary(stream->GetMatchString())+"'";
	//stream
	if(!no_stream_save){
		query += ",E'";
//		for(list<Packet*>::iterator it = stream->GetPacketFirstIt(); it != stream->GetPacketLastIt(); it++){
//			query += T.esc_raw((*it)->GetContent(), (*it)->GetContentSize());
//		}
		query += "'";
	}else{
		query += ",''";
	}
	//stream_org
	if(!no_stream_org_save){
		query += ",''";
	}else{
		query += ",''";
	}

	//MatchPreFilter
	//after_ipfilter, after_prefilter, prefilter_log
	if(stream->GetMatchPreFilterState() != 0){
		oss.str("");
		oss << ", " << stream->GetMatchPreFilterState()->GetAfterIpFilter() << ", " << stream->GetMatchPreFilterState()->GetAfterPreFilter();
		query += oss.str() + ", '" + stream->GetMatchPreFilterState()->GetMatchPreFilterLog() + "'";
	}else{
		query += ", -1, -1, ''";
	}

	query += ");";

#ifdef FILEWRITE_MODE
		file_writer->Write(query);
#endif
#ifdef POSTGRES_MODE
	connection *conn = pgsql->GetConn();
	work T(*conn);
	try{
//		T.exec(query);
//		T.commit();
	}
	catch(const exception &e){
		cerr << e.what() << endl;
	}
	catch(...){
		cerr << "unhandled exception" << endl;
	}
#endif
	oss.str("");

	return;
}

void PgsqlSaver::ProcPacket(Packet * pkt){

	ostringstream oss;

	if(pkt->GetProtocol() != IPPROTO_TCP){return;}
	char ctstamp[100];
	struct timeval tmp_time = pkt->GetTimestamp();
	strftime(ctstamp, 100, "%Y-%m-%d %H:%M:%S", (const struct tm *)localtime(&tmp_time.tv_sec));
	string tstamp = ctstamp;
//	BLUE cout << timetemp << endl;RESET
	/*
	oss << tmp->tm_year+1900 <<"-"<< tmp->tm_mon+1 <<"-"<<tmp->tm_mday <<" "<<tmp->tm_hour<<":"<<tmp->tm_min<<":"<<tmp->tm_sec;
	string tstamp = oss.str();
	*/


	oss.str("");
	oss << "insert into save_packet (id , src_ip ,dst_ip ,src_port ,dst_port ,timestamp , \
	protocol, packet_size, packet_size_org, content_size, flag, content) values (\
	default,'"<< pkt->GetSrcIPStr() <<"','"<<pkt->GetDstIPStr() <<"','"<< pkt->GetSrcPort() <<"','"\
	<<pkt->GetDstPort() <<"','"<< tstamp <<"','"<<pkt->GetProtocol()<<"','"<< pkt->GetPacketSize() <<"','"<< pkt->GetPacketSizeOrg() <<"','"\
	<< pkt->GetContentSize();
	string query = oss.str();

	//flag row
	query += "','";
	if(pkt->GetProtocol() == IPPROTO_TCP){
		if(pkt->GetAck()){ query += "[ACK]"; }
		if(pkt->GetFin()){ query += "[FIN]"; }
		if(pkt->GetSyn()){ query += "[SYN]"; }
		if(pkt->GetRst()){ query += "[RST]"; }
	}

	//query += "',E'"+T.esc_raw(pkt->GetContent(), pkt->GetContentSize())+"');";
	query += "',E'"+escape_binary(pkt->GetContent(), pkt->GetContentSize())+"');";

	connection *conn = pgsql->GetConn();
	work T(*conn);

	try{
//		T.exec(query);
//		T.commit();
	}
	catch(const exception &e){
		cerr << e.what() << endl;
	}
	catch(...){
		cerr << "unhandled exception" << endl;
	}

//	pgsql.ExecSql(query);
	oss.str("");

	return;
}

#endif //USE_POSTGRES
