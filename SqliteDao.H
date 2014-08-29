#pragma once

#include <sqlite3.h>


class SqliteDao
{
    private:
        sqlite3 *conn_;
        sqlite3_stmt *res_;
        int  rc_;
    
    public:
        SqliteDao();
        int Connect(const string& dbname);
        sqlite3_stmt* ExecSql(const string& sql);
        void ExecBatchSql(const string&sql, int limit=2);
        void CreateTableFromFile(const string& file_path);
        void EscapeSingleQuote(string &target);
};
