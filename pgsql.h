/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Pgsql.H,v 5.0 2010-10-13 08:47:16 sin Exp $
*/

#ifndef Pgsql_H
#define Pgsql_H
using namespace pqxx;

class Pgsql
{
	private:
		connection *conn;
		result res;
		int SelectSqlInt(const string& sql);
		map<int,string> SelectSqlIntStr(const string& sql);
		map<string,string> SelectSqlStrStr(const string& sql);
	public:
		Pgsql();
		int Connect(const string& dbname, const string& dbuser = "", const string& dbhost = "", const string& dbpass = "");
		int Disconnect();
		result ExecSql(const string& sql);
		connection *GetConn(){return conn;}
		int CheckFilter();
		int SelectFilter();
		int Proc();
		map<int,string> SelectRegexp();
		map<string,string> SelectConfig();
};

#endif
