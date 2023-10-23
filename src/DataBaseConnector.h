#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <sql.h>
#include <sqlext.h>

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
    std::string name;
    std::string user;
    std::string password;
};

#endif // DATABASECONNECTOR_H
