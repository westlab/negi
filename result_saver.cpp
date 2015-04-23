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
#include "result_saver.h"
#include "global.h"

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

#ifdef FILEWRITE_MODE
            file_writer->Write(query);
#endif
        oss.str("");

    }
}
