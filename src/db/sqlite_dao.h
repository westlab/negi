/*
 * * ** Copyright (C) 2015 Westlab, All Rights Reserved.
 * */

#pragma once

#ifndef DB_SQLITE_DAO_H_
#define DB_SQLITE_DAO_H_

#include <iostream>
#include <sqlite3.h>

using namespace std;

class SqliteDao{
 private:
  sqlite3 *conn_;
  sqlite3_stmt *res_;
  int  rc_;
  string dbname_;

 public:
  SqliteDao();
  int Connect(const string& dbname);
  sqlite3_stmt* ExecSql(const string& sql);
  void ExecBatchSql(const string&sql, int limit = 100000);
  void CreateTableFromFile(const string& file_path);
  void EscapeSingleQuote(string &target);
};

#endif  // DB_SQLITE_DAO_H_
