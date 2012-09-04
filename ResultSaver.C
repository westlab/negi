/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: ResultSaver.C,v 5.6 2012-05-09 17:24:13 sin Exp $
*/
#include "ResultSaver.H"
#include "Global.H"

ResultSaver::ResultSaver(){

	return;
}
ResultSaver::~ResultSaver(){
	return;
}

void ResultSaver::Proc(Packet *pkt){
	for(list<Result *>::iterator it=result_pool->GetResultFirstIt(); it != result_pool->GetResultLastIt(); it++){
		ostringstream oss;
		oss.str("");

		oss << "insert into save_result(id, stream_id, date, rule_id, result) values "\
		<< "NULL" << "," << (*it)->GetParentStream()->GetStreamId() << "," << (*it)->GetFilterId() << "," \
		<< (*it)->GetStreamOffsetSize() << "," << (*it)->GetPacketOffsetSize() << "," << (*it)->GetPacketContentSize() ;

		string query = oss.str();
		
#ifdef USE_POSTGRES
		query += "',E'"+escape_binary((*it)->GetResultString(), (*it)->GetResultSize())+"');";
#endif

#ifdef FILEWRITE_MODE
			file_writer->Write(query);
#endif

#ifdef USE_POSTGRES
#ifdef POSTGRES_MODE
		connection *conn = pgsql->GetConn();
		work T(*conn);
		
		try{
			T.exec(query);
			T.commit();
		}
		catch(const exception &e){
			cerr << e.what() << endl;
		}
		catch(...){
			cerr << "unhandled exception" << endl;
		}
#endif	//POSTGRES_MODE
#endif	//USE_POSTGRES

		oss.str("");
		
	}
}


