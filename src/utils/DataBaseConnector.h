#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <sql.h>
#include <sqlext.h>

#include <fstream>
#include <iostream>
#include <string>

class DataBaseConnector {
 public:
   static DataBaseConnector& getInstance() {
    static DataBaseConnector instance;
    return instance;
  }

  bool connect();
  void disconnect();
  SQLHDBC getConnection();

  bool readConfigFile(const std::string& configFile);
  SQLHENV getHDBC() { return henv; }

 private:
  SQLHENV henv;
  SQLHDBC hdbc;
  DataBaseConnector();
  ~DataBaseConnector();

  std::string dsn;
  std::string dbname;
  std::string user;
  std::string password;
  std::string server;
  int port;
};

#endif  // DATABASECONNECTOR_H
