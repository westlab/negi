#include "Include.H"
#include "Global.H"
#include "Util.H"

#include "SqliteDao.H"

SqliteDao::SqliteDao(){
    return;
}

// Connect to a Sqlite DB
int SqliteDao::Connect(const string& dbname){
   rc_ = sqlite3_open(dbname.c_str(), &conn_);
   if(rc_){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(conn_));
      exit(0);
   }
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
    sqlite3_prepare_v2(conn_, sql.c_str(), -1, &res_, NULL);
    // Excute all SQL 
    sqlite3_step(res_);
    return res_;
}

// Execute write only sql
void SqliteDao::ExecBatchSql(const string &sql, int limit){
    sqlite3_stmt *statement;
    int status=0;
    // Compile SQL
    sqlite3_prepare_v2(conn_, sql.c_str(), -1, &statement, NULL);
    // Excute all SQL 
    int loop=0;
    while (1){
        status = sqlite3_step(statement);
        if(status == SQLITE_ERROR){
            fprintf(stderr, "SQL gets an error.: %s\n", sqlite3_errmsg(conn_));
            break;
        }else if(status == SQLITE_DONE){
            //SQL are executed without errors
            break;
        }else if(status == SQLITE_BUSY){
            cout << "sqlite is busy" << endl;
            cout << sqlite3_errmsg(conn_) << endl;
        }else if(status == SQLITE_ROW){
            cout << "sql returns row. this Operator is supposed to be used for read only operation" << endl;
        }else if(status == SQLITE_MISUSE){
            cout << "sql is used in wrong way" << endl;
            cout << sqlite3_errmsg(conn_) << endl;
            sleep(2);
        }else{
            cout << "sql returns unkown error" << endl;
            cout << sqlite3_errmsg(conn_) << endl;
        }
        if (loop++>limit){
            cout << "sqlite execution loop reached max." << endl;
            break;
        }
    }
    sqlite3_finalize(statement);
}

// Get SQL from a file and create table.
void SqliteDao::CreateTableFromFile(const string &file_path){
	ifstream is;
	is.open(file_path.c_str());

	if(!is){
		cout << "Config: Can't open " << file_path << "."<< endl;
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
