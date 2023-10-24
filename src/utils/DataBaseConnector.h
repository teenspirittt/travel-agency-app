#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <sql.h>
#include <sqlext.h>

#include <fstream>
#include <iostream>
#include <string>

class DataBaseConnector {
 public:
  DataBaseConnector();
  ~DataBaseConnector();

  bool connect();
  void disconnect();
  SQLHDBC getConnection();

  bool readConfigFile(const std::string& configFile);
  SQLHENV getHDBC() { return hdbc; }

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

#endif  // DATABASECONNECTOR_H
