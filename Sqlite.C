#include "Include.H"
#include "Global.H"

#include "Sqlite.H"

Sqlite::Sqlite(){
    return;
}

// Connect to a Sqlite DB
int Sqlite::Connect(const string& dbname){
   rc_ = sqlite3_open(dbname, &conn_);
   if(rc_){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(conn_));
      exit(0);
   }
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
sqlite3_stmt* Sqlite::ExecSql(const string &sql){
    // Compile SQL
    sqlite3_prepare_v2(conn_, sql, strlen(query), &res_, NULL);
    // Excute SQL 
    sqlite3_step(res_)
    return res_
}

// Get SQL from a file and create table.
void Sqlite::CreateTableFromFile(const string &file_path){
	ifstream is;
	is.open(file_path.c_str());

	if(!is){
		cout << "Config: Can't open " << file_path << "."<< endl;
		exit(1);
	}

    string sql;
    for (string line; getline(is, line);){
        sql += line;
    }
    statement = ExcuteSql(sql);
    sqlite3_finalize(statement);
}
