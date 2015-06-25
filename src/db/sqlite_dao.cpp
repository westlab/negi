#include "sqlite_dao.h"
#include <unistd.h>
#include <fstream>
#include "glog/logging.h"
#include "util.h"

SqliteDao::SqliteDao(){
    return;
}

// Connect to a Sqlite DB
int SqliteDao::Connect(const string& dbname){
   rc_ = sqlite3_open(dbname.c_str(), &conn_);
    dbname_ = dbname;
   if(rc_){
      LOG(ERROR) << "Can't open database: " << sqlite3_errmsg(conn_);
      exit(0);
   }
    // set WAL mode
    string sql = "PRAGMA journal_mode=WAL;";
    sqlite3_stmt *statement;
    sqlite3_prepare_v2(conn_, sql.c_str(), -1, &statement, NULL);
    sqlite3_step(statement);
    sqlite3_finalize(statement);
    // sqlite3_close(conn_);
   return 1;
}

// Execute SQL
// e.g
// statement = sqlite->Execsql(sql);
// while(sqlite3_step(statement) == SQLITE_ROW){
//      int id = sqlite3_column_int(statement, 0);
//      const unsigned char * name = sqlite3_column_text(statement, 1);
//      ...
// }
// // release sqlite3_stmt object
// sqlite3_finalize(statement);
sqlite3_stmt* SqliteDao::ExecSql(const string &sql){
    // Compile SQL
    sqlite3_stmt *statement;
    sqlite3_prepare_v2(conn_, sql.c_str(), -1, &statement, NULL);
    // Excute all SQL
    sqlite3_step(statement);
    sqlite3_finalize(statement);
    return statement;
}

// Execute write only sql
void SqliteDao::ExecBatchSql(const string &sql, int limit){
    // Open and close db every time is not good solution in terms of
    // overhaed of open and close operation.
    // It seems that lock is not released when db is open in runtime.
    // sqlite3 *conn;
    rc_ = sqlite3_open(dbname_.c_str(), &conn_);
    string wal = "PRAGMA journal_mode=WAL;";
    sqlite3_stmt *statement;
    sqlite3_prepare_v2(conn_, wal.c_str(), -1, &statement, NULL);
    sqlite3_finalize(statement);

    int status=0;
    // Compile SQL
    sqlite3_prepare_v2(conn_, sql.c_str(), -1, &statement, NULL);
    // Excute all SQL
    int loop=0;
    while (1){
        status = sqlite3_step(statement);
        if(status == SQLITE_ERROR){
            LOG(ERROR) << "SQL gets an error.: " <<  sqlite3_errmsg(conn_);
            break;
        }else if(status == SQLITE_DONE){
            //SQL are executed without errors
            break;
        }else if(status == SQLITE_BUSY){
            LOG(INFO) << "sqlite is busy";
            LOG(INFO) << sqlite3_errmsg(conn_);
        }else if(status == SQLITE_ROW){
            LOG(INFO) << "sql returns row. this Operator is supposed to be used for read only operation";
        }else if(status == SQLITE_MISUSE){
            LOG(INFO) << "sql is used in wrong way";
            LOG(INFO) << sqlite3_errmsg(conn_);
            sleep(2);
        }else{
            LOG(INFO) << "sql returns unkown error";
            LOG(INFO) << sqlite3_errmsg(conn_);
        }
        if (loop++>limit){
            LOG(INFO) << "sqlite execution loop reached max.";
            break;
        }
    }
    sqlite3_finalize(statement);
    // sqlite3_close(conn_);
}

// Get SQL from a file and create table.
void SqliteDao::CreateTableFromFile(const string &file_path){
    ifstream is;
    is.open(file_path.c_str());

    if(!is){
        LOG(ERROR) << "Config: Can't open " << file_path;
        exit(1);
    }

    string sqls;
    for (string line; getline(is, line);){
        sqls += line;
    }

    //Parse each sql
    string delimiter = ";";

    size_t pos = 0;
    string sql;
    while ((pos = sqls.find(delimiter)) != string::npos) {
        sql = sqls.substr(0, pos);
        sqls.erase(0, pos + delimiter.length());
        ExecBatchSql(sql, 2);
    }
}

void SqliteDao::EscapeSingleQuote(string &target){
    find_and_replace(target, "'", "''");
}
