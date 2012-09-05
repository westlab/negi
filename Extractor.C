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

#include "Include.H"
#include "Global.H"
#include "Stream.H"
#include "Extractor.H"

Extractor::Extractor(){
	return;
}


void Extractor::Proc(Packet *pkt){
//	pkt->GetStream()->GetResultIt();

	if(pkt->GetStream() != 0){
		for(list<PapaResult*>::iterator it = pkt->GetStream()->GetPapaResultListFirstIt(); it != pkt->GetStream()->GetPapaResultListLastIt();){
			if((*it)->GetPRule()->GetSaveFlag()){
//				pkt->GetStream()->SetSaveFlag();

				u_int result_start_num = (*it)->GetPatLen() + (*it)->GetPlaceOfPacket();
				u_int result_end_num = result_start_num + RESULT_SIZE;


/*				if((*it)->GetPlaceOfPacket() < 0 || pkt->GetL7ContentSize() == 0 || (*it)->GetPlaceOfPacket() > pkt->GetL7ContentSize()){
				cerr << "packet of place < 0!!" <<endl;

				}else 
				*/
				if((u_int)(*it)->GetPlaceOfPacket() > pkt->GetL7ContentSize() ){
					ERROR_DEBUG(cout << "something error on extractor!!" << endl;)
					/*
					cout << "this is result!!--------------------" << endl;
					pkt->Show();
					cout << "TimeStamp: "; observer.ShowMem(pkt->GetTimestamp()) ;
					cout << "Stream p: "<< pkt->GetStream() << endl;
					cout << "Rule id: "<< (*it)->GetRuleId() << endl;
					cout << "Rule : "<< (*it)->GetPRule()->GetPreFilterPattern() << endl;
					cout << "Pattern Length: "<< (*it)->GetPatLen() << endl;
					cout << "Stream Size: " << pkt->GetStream()->GetRetrievedContentSize() << endl;
					cout << "Packet Size: " << pkt->GetL7ContentSize() << endl;
					cout << "Packet Place: " << (*it)->GetPlaceOfPacket() << endl;
					cout << "Packet Offset: " << (*it)->GetResultOffset() << endl;
					cout << "ResultStart: " << result_start_num << endl;
					cout << "ResultEnd: " << result_end_num << endl;
					cout << "Flag: " << (*it)->GetFinished() << endl;
					cout << "Packet num: " << pkt->GetStream()->GetPacketNum() << endl;
					*/
					}else 

				if((*it)->GetResultOffset() > 0){
					//this means results crosses packets.
					(*it)->SetResultString(pkt->GetL7Content() , 0, RESULT_SIZE - (*it)->GetResultOffset());
					(*it)->SetResultSize(RESULT_SIZE);
					(*it)->SetFinished(1);
/*
				cout << "this is result!!--------------------" << endl;
				pkt->Show();
				cout << "TimeStamp: "; observer.ShowMem(pkt->GetTimestamp()) ;
				cout << "Stream p: "<< pkt->GetStream() << endl;
				cout << "Rule id: "<< (*it)->GetRuleId() << endl;
				cout << "Rule : "<< (*it)->GetPRule()->GetPreFilterPattern() << endl;
				cout << "Pattern Length: "<< (*it)->GetPatLen() << endl;
				cout << "Stream Size: " << pkt->GetStream()->GetRetrievedContentSize() << endl;
				cout << "Packet Size: " << pkt->GetL7ContentSize() << endl;
				cout << "Packet Place: " << (*it)->GetPlaceOfPacket() << endl;
				cout << "Packet Offset: " << (*it)->GetResultOffset() << endl;
				cout << "ResultStart: " << result_start_num << endl;
				cout << "ResultEnd: " << result_end_num << endl;
				cout << "Flag: " << (*it)->GetFinished() << endl;

*/
				}else{
					if(result_end_num < pkt->GetL7ContentSize()){
						(*it)->SetResultString(pkt->GetL7Content() + result_start_num, 0, RESULT_SIZE);
						(*it)->SetResultSize(RESULT_SIZE);
						(*it)->SetFinished(1);
					}else if( pkt->GetL7ContentSize() < result_start_num ){
						cout << "PaketSize < Result start!!" << endl;
						(*it)->SetFinished(1);
					}else{
			//			cout << "PlaceOfPacket overs packet size!!" << endl;

						(*it)->SetResultString(pkt->GetL7Content() + result_start_num, 0, pkt->GetL7ContentSize()- result_start_num);
						(*it)->SetResultOffset(result_end_num - pkt->GetL7ContentSize());
						(*it)->SetFinished(0);
					}
				}
			
				if((*it)->GetFinished()){
					//Lets save it to PGSQL
/*
				cout << "this is result!!--------------------" << endl;
				pkt->Show();
//				cout << "TimeStamp: "; observer.ShowMem(pkt->GetTimestamp()) ;
				cout << "Stream p: "<< pkt->GetStream() << endl;
				cout << "Rule id: "<< (*it)->GetRuleId() << endl;
				cout << "Rule : "<< (*it)->GetPRule()->GetPreFilterPattern() << endl;
				cout << "Pattern Length: "<< (*it)->GetPatLen() << endl;
				cout << "Stream Size: " << pkt->GetStream()->GetRetrievedContentSize() << endl;
				cout << "Packet Size: " << pkt->GetL7ContentSize() << endl;
				cout << "Packet Place: " << (*it)->GetPlaceOfPacket() << endl;
				cout << "Packet Offset: " << (*it)->GetResultOffset() << endl;
				cout << "ResultStart: " << result_start_num << endl;
				cout << "ResultEnd: " << result_end_num << endl;
				cout << "Flag: " << (*it)->GetFinished() << endl;

*/

					ostringstream oss;
					oss.str("");

					oss << "insert into save_result(id, stream_id, rule_id, pattern, pattern_len, place, result) values "\
					<< "(default,'" << pkt->GetStream()->GetStreamId() << "','" << (*it)->GetRuleId() << "','" \
					<< (*it)->GetPRule()->GetPreFilterPattern() << "','" << (*it)->GetPatLen() << "','" << (*it)->GetPlaceOfPacket()  ;

					string query = oss.str();
					
#ifdef USE_POSTGRES
					query += "',E'"+escape_binary((*it)->GetResultString(), (*it)->GetResultSize())+"');";
#endif

//					cout << query << endl;

#ifdef FILEWRITE_MODE
						file_writer->Write(query);
#endif

#ifdef RIVER_MODE
					river_gate->ProcResult( pkt->GetStream()->GetStreamId(), (*it)->GetRuleId(), (*it)->GetResultString(), (*it)->GetResultSize());
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
					it = pkt->GetStream()->DeletePapaResultIt(it);
				}else{
					it++;
				}
			}else{
				it++;
			}
		}

	}
}
