#include <iostream>
#include <dlfcn.h>
#include "../include/database.hpp"
#include "../include/network.hpp"

void testDatabase()
{
    QueryManager db;

    db.openDatabase("data.db");
//    db.createTable();
    db.querySQL("PRAGMA table_info(USERS);");
//    db.querySQL("INSERT INTO USERS (NAME,PASSWORD,CONTACT) "  \
//          "VALUES ('Toto', 'toto*1', 'Tata, Titi');");
//    db.querySQL("INSERT INTO USERS (NAME,PASSWORD,CONTACT) "  \
//          "VALUES ('Tata', 'tata*1', 'Toto, Titi');");
    db.querySQL("SELECT * FROM USERS WHERE NAME = 'Toto'");
    db.querySQL("SELECT * FROM USERS WHERE NAME = 'Tata'");
    db.closeDatabase();
}

int main()
{
    int nb = 0;
    AsioNetwork network(3005);
    network.openConnections();
    while (1)
    {
        sleep(5);
//        network.sendPackageToClient(std::to_string(nb));
        nb++;
    }
    network.closeConnections();
//    testDatabase();
    return 0;
}