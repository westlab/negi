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

				struct timeval tmp_time = pkt->GetStream()->GetTimestamp();
				struct tm *tmp = localtime(&tmp_time.tv_sec);
				ostringstream oss;
				oss << tmp->tm_year+1900 <<"-"<< tmp->tm_mon+1 <<"-"<<tmp->tm_mday <<" "<<tmp->tm_hour<<":"<<tmp->tm_min<<":"<<tmp->tm_sec;
				string tstamp = oss.str();

				string src_ip =inet_ntoa(pkt->GetSrcIP());
				string dst_ip =inet_ntoa(pkt->GetDstIP());


//timestamp
				struct timeval tmp_time = pkt->GetStream()->GetTimestamp();
				struct tm *tmp = localtime(&tmp_time.tv_sec);
				ostringstream oss;
				oss << tmp->tm_year+1900 <<"-"<< tmp->tm_mon+1 <<"-"<<tmp->tm_mday <<" "<<tmp->tm_hour<<":"<<tmp->tm_min<<":"<<tmp->tm_sec;
				string tstamp = oss.str();

				string src_ip =inet_ntoa(pkt->GetSrcIP());
				string dst_ip =inet_ntoa(pkt->GetDstIP());


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
					}else if((*it)->GetResultOffset() > 0){
					//this means results crosses packets.

					if( (RESULT_SIZE - (*it)->GetResultOffset()) <= pkt->GetL7ContentSize()){
						(*it)->SetResultString(pkt->GetL7Content() , 0, RESULT_SIZE - (*it)->GetResultOffset());
						(*it)->SetResultSize(RESULT_SIZE);
						(*it)->SetFinished(1);
					}else{
						(*it)->SetResultString(pkt->GetL7Content() , 0, pkt->GetL7ContentSize());
						(*it)->SetResultSize(pkt->GetL7ContentSize());
						(*it)->SetResultOffset(RESULT_SIZE - (*it)->GetResultOffset() - pkt->GetL7ContentSize());
//						(*it)->SetFinished(0);
					}

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
///*
				cout << "this is result!!--------------------" << endl;
//				pkt->Show();
			//	cout << "TimeStamp: "; observer.ShowMem(pkt->GetTimestamp()) ;
			//	cout << "Stream p: "<< pkt->GetStream() << endl;
			//	cout << "Rule id: "<< (*it)->GetRuleId() << endl;
				cout << "Rule : "<< (*it)->GetPRule()->GetPreFilterPattern() << endl;
			//	cout << "Pattern Length: "<< (*it)->GetPatLen() << endl;
			//	cout << "Stream Size: " << pkt->GetStream()->GetRetrievedContentSize() << endl;
			//	cout << "Packet Size: " << pkt->GetL7ContentSize() << endl;
			//	cout << "Packet Place: " << (*it)->GetPlaceOfPacket() << endl;
			//	cout << "Packet Offset: " << (*it)->GetResultOffset() << endl;
			//	cout << "ResultStart: " << result_start_num << endl;
			//	cout << "ResultEnd: " << result_end_num << endl;
			//	cout << "Flag: " << (*it)->GetFinished() << endl;
				if(pkt->GetStream()->GetHttpCompress()==2){
				BLUE	cout << "HTTP Encode: " << "GZIP--------------" << endl;RESET
//					cout << pkt->GetL7Content() << endl;
//				BLUE	cout << "-------------------------------" << endl;RESET
				}else{
					cout << "HTTP Encode: " << "None" << endl;
				}


				cout << "Source IP,port:      " << inet_ntoa(pkt->GetSrcIP()) << ","<< pkt->GetSrcPort()<< endl;
				cout << "Destination IP,port: " << inet_ntoa(pkt->GetDstIP()) << "," <<pkt->GetDstPort()<< endl;
				cout << "ResultString: ";
				RED cout <<(*it)->GetPRule()->GetPreFilterPattern();
				YELLOW cout << escape_binary((*it)->GetResultString(), (*it)->GetResultSize()) << endl; RESET
				cout << (*it)->GetResultString() << endl;
				if(!strcmp((*it)->GetPRule()->GetPreFilterPattern().c_str(),"VIRUS")){
					URED cout << "VIRUS DETECTED!! Shut out :"<< inet_ntoa(pkt->GetSrcIP()) << endl; RESET
					RED; system("./nii-filter -A 11.11.11.1 -I xe-0/0/0");RESET
				}

				cout << "------------------------------------" << endl;
//*/

					ostringstream oss;
					oss.str("");

					oss << "insert into save_result(id, stream_id, rule_id, pattern, pattern_len, place,timestamp, src_ip, dst_ip, src_port, dst_port ,result) values "\
					<< "(default,'" << pkt->GetStream()->GetStreamId() << "','" << (*it)->GetRuleId() << "','" \
					<< (*it)->GetPRule()->GetPreFilterPattern() << "','" << (*it)->GetPatLen() << "','" << (*it)->GetPlaceOfPacket() << "','" \
					<< tstamp << "','" << src_ip << "','" << dst_ip << "','" \
					<< pkt->GetSrcPort() << "','" << pkt->GetDstPort();

					string query = oss.str();

#ifdef USE_POSTGRES
					query += "','"+escape_binary((*it)->GetResultString(), (*it)->GetResultSize())+"');";
					cout << query << endl;

#else
					char * temp = (char *)malloc(sizeof(char)* 100);
					memcpy(temp, (char *)((*it)->GetResultString()), 99);
					temp[99] = '\0';
					query += "','";
					query += temp;
					query += "');";
					free(temp);
#endif
					cout << query << endl;

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
