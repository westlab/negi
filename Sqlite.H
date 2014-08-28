#include <sqlite3.h>


class Sqlite
{
    private:
        sqlite3 *conn_;
        sqlite3_stmt *res_;
        int  rc_;
    
    public:
        Sqlite();
        int Connect(const string& dbname);
        sqlite3_stmt* ExecSql(const string& sql);
        void CreateTableFromFile(const string& file_path);
}
