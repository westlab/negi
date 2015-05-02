/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Extractor.C,v 5.20 2012-05-09 17:24:12 sin Exp $
*/

#include "include.h"
#include "global.h"
#include "stream.h"
#include "extractor.h"
#include "glog/logging.h"

Extractor::Extractor() {
    return;
}

void Extractor::Proc(Packet *pkt) {
    if (pkt->GetStream() != 0) {
        int end_flag = 0;  // if this packet is the end of stream, end_flag is 1
        if (pkt->ExFlag()) {
            end_flag = 1;
        }
        for (list<PapaResult*>::iterator it = pkt->GetStream()->GetPapaResultListFirstIt();
             it != pkt->GetStream()->GetPapaResultListLastIt();) {
            int error_flag = 0;
            if ((*it)->GetPRule()->GetSaveFlag()) {
                u_int result_start_num = (*it)->GetPlaceOfPacket() + 1;  // for Aho Corasick
                u_int result_end_num = result_start_num + RESULT_SIZE;

                struct timeval tmp_time = pkt->GetStream()->GetTimestamp();
                struct tm *tmp = localtime(&tmp_time.tv_sec);
                ostringstream oss;
                oss << tmp->tm_year+1900 <<"-"<< tmp->tm_mon+1
                    <<"-"<< tmp->tm_mday
                    <<" "<< tmp->tm_hour
                    <<":"<< tmp->tm_min
                    <<":"<< tmp->tm_sec;
                string tstamp = oss.str();

                if ((*it)->GetResultOffset() > 0) {
                    // this means results crosses packets.
                    if ((*it)->GetResultOffset() <= pkt->GetL7ContentSize()) {
                        (*it)->SetResultString(pkt->GetL7Content(),
                                               RESULT_SIZE - (*it)->GetResultOffset(),
                                               (*it)->GetResultOffset());
                        (*it)->SetResultSize(RESULT_SIZE);
                        (*it)->SetFinished(1);
                    } else {
                        (*it)->SetResultString(pkt->GetL7Content(),
                                               RESULT_SIZE - (*it)->GetResultOffset(),
                                               pkt->GetL7ContentSize());
                        (*it)->SetResultSize(RESULT_SIZE
                         - (*it)->GetResultOffset()
                         + pkt->GetL7ContentSize());
                        (*it)->SetResultOffset((*it)->GetResultOffset() - pkt->GetL7ContentSize());
                        if (end_flag) {
                            (*it)->SetFinished(1);
                        } else {
                            (*it)->SetFinished(0);
                        }
                    }

                    LOG(INFO) << "Stream p: "<< pkt->GetStream();
                    LOG(INFO) << "Rule id: "<< (*it)->GetRuleId();
                    LOG(INFO) << "Rule : "<< (*it)->GetPRule()->GetPreFilterPattern();
                    LOG(INFO) << "Pattern Length: "<< (*it)->GetPatLen();
                    LOG(INFO) << "Stream Size: " << pkt->GetStream()->GetRetrievedContentSize();
                    LOG(INFO) << "Packet Size: " << pkt->GetL7ContentSize();
                    LOG(INFO) << "Packet Place: " << (*it)->GetPlaceOfPacket();
                    LOG(INFO) << "Packet Offset: " << (*it)->GetResultOffset();
                    LOG(INFO) << "ResultStart: " << result_start_num;
                    LOG(INFO) << "ResultEnd: " << result_end_num;
                    LOG(INFO) << "Flag: " << (*it)->GetFinished();
                } else if ( (*it)->GetPlaceOfPacket()
                            > static_cast<int>(pkt->GetL7ContentSize()) ) {
                    error_flag = 1;

                    LOG(ERROR) << "Stream p: "<< pkt->GetStream();
                    LOG(ERROR) << "Rule id: "<< (*it)->GetRuleId();
                    LOG(ERROR) << "Rule : "<< (*it)->GetPRule()->GetPreFilterPattern();
                    LOG(ERROR) << "Pattern Length: "<< (*it)->GetPatLen();
                    LOG(ERROR) << "Stream Size: " << pkt->GetStream()->GetRetrievedContentSize();
                    LOG(ERROR) << "Packet Size: " << pkt->GetL7ContentSize();
                    LOG(ERROR) << "Packet Place: " << (*it)->GetPlaceOfPacket();
                    LOG(ERROR) << "Packet Offset: " << (*it)->GetResultOffset();
                    LOG(ERROR) << "ResultStart: " << result_start_num;
                    LOG(ERROR) << "ResultEnd: " << result_end_num;
                    LOG(ERROR) << "Flag: " << (*it)->GetFinished();
                    LOG(ERROR) << "Packet num: " << pkt->GetStream()->GetPacketNum();
                } else {
                    if (result_end_num < pkt->GetL7ContentSize()) {
                        (*it)->SetResultString(pkt->GetL7Content() + result_start_num,
                                               0,
                                               RESULT_SIZE);
                        (*it)->SetResultSize(RESULT_SIZE);
                        (*it)->SetFinished(1);
                    } else {
                        (*it)->SetResultString(pkt->GetL7Content() + result_start_num,
                                               0,
                                               pkt->GetL7ContentSize() - result_start_num);
                        (*it)->SetResultSize(RESULT_SIZE
                                             - (result_end_num - pkt->GetL7ContentSize()));
                        (*it)->SetResultOffset(result_end_num - pkt->GetL7ContentSize());
                        if (end_flag) {
                            (*it)->SetFinished(1);
                        } else {
                            (*it)->SetFinished(0);
                        }
                    }
                }

                if ((*it)->GetFinished()) {
                    // Lets save it to PGSQL
                    LOG(INFO) << "Extreacted result";
                    LOG(INFO) << "Stream p: "<< pkt->GetStream();
                    LOG(INFO) << "Rule id: "<< (*it)->GetRuleId();
                    LOG(INFO) << "Rule : "<< (*it)->GetPRule()->GetPreFilterPattern();
                    LOG(INFO) << "Pattern Length: "<< (*it)->GetPatLen();
                    LOG(INFO) << "Stream Size: " << pkt->GetStream()->GetRetrievedContentSize();
                    LOG(INFO) << "Packet Size: " << pkt->GetL7ContentSize();
                    LOG(INFO) << "Packet Place: " << (*it)->GetPlaceOfPacket();
                    LOG(INFO) << "Packet Offset: " << (*it)->GetResultOffset();
                    LOG(INFO) << "ResultStart: " << result_start_num;
                    LOG(INFO) << "ResultEnd: " << result_end_num;
                    LOG(INFO) << "Flag: " << (*it)->GetFinished();
                    if (pkt->GetStream()->GetHttpCompress() == 2) {
                        LOG(INFO) << "HTTP Encode: " << "GZIP--------------";
                    } else {
                        LOG(INFO) << "HTTP Encode: " << "None";
                    }

                    LOG(INFO) << "Source IP,port:      " << pkt->GetSrcIPStr()
                              << ","<< pkt->GetSrcPort() << endl;
                    LOG(INFO) << "Destination IP,port: " << pkt->GetDstIPStr()
                              << ","                     << pkt->GetDstPort()
                              << endl;
                    LOG(INFO) << "ResultString: "
                              << (*it)->GetPRule()->GetPreFilterPattern();
                    LOG(INFO) << (*it)->GetResultString();
                    LOG(INFO) << "------------------------------------";

                    LOG(INFO) << "MAC Addr:"             << pkt->GetDstMacAddr();

                    ostringstream oss;
                    oss.str("");


                    oss << "insert into save_result"
                        << "(stream_id, rule_id, pattern, pattern_len,"
                        << " place,timestamp, src_ip, dst_ip, src_port,"
                        << " dst_port, src_mac_addr, dst_mac_addr, result) values "\
                        << "('" << pkt->GetStream()->GetStreamId()
                        << "','" << (*it)->GetRuleId() << "','" \
                        << (*it)->GetPRule()->GetPreFilterPattern() << "','"
                        << (*it)->GetPatLen() << "','" << (*it)->GetPlaceOfPacket() << "','" \
                        << tstamp << "','" <<  pkt->GetSrcIPStr()
                        << "','" <<  pkt->GetDstIPStr() << "','" \
                        << pkt->GetSrcPort() << "','" << pkt->GetDstPort()
                        << "','" << pkt->GetSrcMacAddr() << "','" << pkt->GetDstMacAddr();

                    string query = oss.str();

                    char * temp = reinterpret_cast<char *>(malloc(sizeof(char)* RESULT_SIZE));
                    memcpy(temp,
                           reinterpret_cast<char *>(((*it)->GetResultString())), RESULT_SIZE - 1);
                    temp[RESULT_SIZE - 1] = '\0';
                    query += "','";
                    string hit_result(temp);
                    sqlite_dao->EscapeSingleQuote(hit_result);
                    query += hit_result;
                    query += "');";
                    free(temp);
                    LOG(INFO) << query;
                    sqlite_dao->ExecBatchSql(query);

#ifdef FILEWRITE_MODE
                    file_writer->Write(query);
#endif
                    oss.str("");
                    it = pkt->GetStream()->DeletePapaResultIt(it);
                } else if (error_flag) {
                    it = pkt->GetStream()->DeletePapaResultIt(it);
                } else {
                    it++;
                }
            } else {
                it++;
            }
        }
    }
}
