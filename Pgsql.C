/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Pgsql.C,v 5.3 2012-05-09 17:24:13 sin Exp $
*/

#include "Include.H"
#include "Global.H"

#ifdef USE_POSTGRES

#include "Pgsql.H"

Pgsql::Pgsql(){
	return;
}


int Pgsql::Connect(const string& dbname, const string& dbuser , const string& dbhost , const string& dbpass){
	string connector("dbname=" + dbname);
	if (dbuser != "") {
		connector += " user=" + dbuser;
	}
	if (dbhost != "") {
		connector += " host=" + dbhost;
	}

	if (dbpass != "") {
		connector += " password=" + dbpass;
	}

	try{
		conn = new connection(connector);
	}
	catch(const exception &e){
		cerr << e.what() << endl;
		exit(1);
	}

	return 0;
}
result Pgsql::ExecSql(const string& sql){
	try{
		work T(*conn);
			res = T.exec(sql);
		T.commit();
	}
	catch(const exception &e){
		cerr << e.what() << endl;
		exit(1);
	}
	catch(...){
		cerr << "unhandled exception" << endl;
		exit(1);
	}
	return res;
	
}

int Pgsql::SelectSqlInt(const string& sql){
	map<int,string> res;
	try{
		work T(*conn);
		result R(T.exec(sql));
		result::const_iterator c= R.begin();
		return c[0].as(int());
	}
	catch (const exception& e){
		cerr << e.what() << endl;
	}
	catch (...){
		cerr << "unhandled exception" << endl;
	}

	return 0;
}

map<int, string> Pgsql::SelectSqlIntStr(const string& sql){
	map<int,string> res;
	try{
		work T(*conn);
		result R(T.exec(sql));
		for( result::const_iterator c = R.begin(); c != R.end(); ++c ) {
			res[c[0].as(int())] = c[1].as(string());
		}
	return res;	
	}
	catch (const exception& e){
		cerr << e.what() << endl;
	}
	catch (...){
		cerr << "unhandled exception" << endl;
	}
	return res;	
}

map<string, string> Pgsql::SelectSqlStrStr(const string& sql){
	map<string,string> res;
	try{
		work T(*conn);
		result R(T.exec(sql));
		for( result::const_iterator c = R.begin(); c != R.end(); ++c ) {
			res[c[0].as(string())] = c[1].as(string());
		}
	return res;
	}
	catch (const exception& e){
		cerr << e.what() << endl;
	}
	catch (...){
		cerr << "unhandled exception" << endl;
	}
	return res;
}
int Pgsql::Disconnect(){
	conn->disconnect();
	return 0;
}


map<int,string> Pgsql::SelectRegexp(){
	return SelectSqlIntStr("select id,regexp from filter order by id");
	
}
map<string,string> Pgsql::SelectConfig(){
	return SelectSqlStrStr("select name,value from config");
}

int Pgsql::CheckFilter(){
	if(SelectSqlInt("select updated from update_check where value = 'filter'")){
		ExecSql("update update_check set updated = 0 where value = 'filter'");
		return 1;
	}
	return 0;
}

int Pgsql::Proc(){
	return 0;
}

#endif
