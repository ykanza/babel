#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>

class QueryManager
{
private:
    sqlite3 *db; // A changer en unique pointer
public:
    QueryManager();
    ~QueryManager();

    bool openDatabase(const std::string &filename);
    void closeDatabase();
    bool createTable();
    bool querySQL(const std::string &sql);
};

#endif