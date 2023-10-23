#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <sql.h>
#include <sqlext.h>
#include <string>
#include <iostream>
#include <fstream>


class DataBaseConnector {
public:
    DataBaseConnector();
    ~DataBaseConnector();

    bool connect();
    void disconnect();
    SQLHDBC getConnection();

    bool readConfigFile(const std::string& configFile);

private:
    SQLHENV henv;
    SQLHDBC hdbc;
   
    std::string dsn;
    std::string dbname;
    std::string user;
    std::string password;
    std::string server;
    int port;
};

#endif // DATABASECONNECTOR_H
