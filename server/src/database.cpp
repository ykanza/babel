#include "../include/database.hpp"
#include <string>
#include <iostream>

QueryManager::QueryManager()
{
}

QueryManager::~QueryManager()
{
    closeDatabase();
}

bool QueryManager::openDatabase(const std::string &filename)
{
    int rc = sqlite3_open(filename.c_str(), &this->db);
   
    if (rc)
        return false;
    return true;
}

void QueryManager::closeDatabase()
{
    sqlite3_close(this->db);
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   for(int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = ";
        printf("%s\n", argv[i] ? argv[i] : "NULL");
        // A CHANGER FORMAT CPP / ! \  //
   }
   std::cout << std::endl;
   return 0;
}

bool QueryManager::createTable()
{
    std::string sql = "CREATE TABLE USERS("  \
      "ID INTEGER PRIMARY KEY," \
      "NAME           TEXT    NOT NULL," \
      "PASSWORD       TEXT    NOT NULL," \
      "CONTACT        TEXT );";
    int rc = sqlite3_exec(this->db, sql.c_str(), callback, 0, nullptr);
    
    if( rc != SQLITE_OK )
        return false;
    return true;
}

bool QueryManager::querySQL(const std::string &sql)
{
    int rc = sqlite3_exec(this->db, sql.c_str(), callback, 0, nullptr);

    if( rc != SQLITE_OK )
        return false;
    return true;
}