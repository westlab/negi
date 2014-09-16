/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: PgsqlRuleLoader.C,v 5.6 2012-05-09 17:24:13 sin Exp $
*/

#include "Include.H"
#include "Global.H"
#include "Stream.H"

#ifdef USE_POSTGRES
#include "PgsqlRuleLoader.H"

PgsqlRuleLoader::PgsqlRuleLoader(){
	return;
}


int PgsqlRuleLoader::Proc(){
	rule_pool->Clear();

	string	sql = 	"select id,owner,timestamp, ";
			sql +=	"src_ip, src_netmask ,src_port, ";
			sql +=	"dst_ip, dst_netmask ,dst_port, ";
			sql +=	"regexp, prefilter_pattern, ";
			sql +=	"prefilter_offset, prefilter_depth, ";
			sql +=	"save_flag ";
			sql +=	" from rule order by id";
	try{
		result res = pgsql->ExecSql(sql);
		for( result::const_iterator c = res.begin(); c != res.end(); ++c ) {
			rule_pool->AddRule(	c[0].as(u_int()), c[1].as(string()), c[2].as(string()), c[3].as(string()), \
			c[4].as(string()), c[5].as(u_int()), c[6].as(string()), c[7].as(string()), c[8].as(u_int()), \
			c[9].as(string()),c[10].as(string()),c[11].as(u_int()), c[12].as(u_int()), c[13].as(u_int()));

		}
	}
	catch (const exception& e){
		cerr << e.what() << endl;
	}
	catch (...){
		cerr << "unhandled exception" << endl;
	}

	sql = 	"select max(id) from save_stream";
	try{
		result res = pgsql->ExecSql(sql);
		for( result::const_iterator c = res.begin(); c != res.end(); ++c ) {
			stream_id_prev = c[0].as(u_int());
//			cout << "STREAM MAX ID: " << stream_id_prev << endl;
		}
	}
	catch (const exception& e){
		cerr << e.what() << endl;
	}
	catch (...){
		cerr << "unhandled exception" << endl;
	}

	

	return 0;	
}
int PgsqlRuleLoader::UpdateCheck(){
	int update_flag;
	string sql;
	sql = 	"select updated from updated_check where value = 'rule'";
	try{
		result res = pgsql->ExecSql(sql);
		for( result::const_iterator c = res.begin(); c != res.end(); ++c ) {
			update_flag = c[0].as(u_int());
//			cout << "STREAM MAX ID: " << stream_id_prev << endl;
		}
	}
	catch (const exception& e){
		cerr << e.what() << endl;
	}
	catch (...){
		cerr << "unhandled exception" << endl;
	}
	if(update_flag == 1){
		cout << "Rule is updated!!" << endl;
		sql = 	"update updated set updated = 0 where value = 'rule'";
		try{
			result res = pgsql->ExecSql(sql);
		}
		catch (const exception& e){
			cerr << e.what() << endl;
		}
		catch (...){
			cerr << "unhandled exception" << endl;
		}
	}

	return update_flag;
}


#endif //USE_POSTGRES
