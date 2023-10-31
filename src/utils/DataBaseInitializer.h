#ifndef DATABASE_INITIALIZER_H
#define DATABASE_INITIALIZER_H

#include <iostream>

#include "DataBaseConnector.h"

class DataBaseInitializer {
 public:
  explicit DataBaseInitializer(DataBaseConnector& connector);

  bool initializeDataBase();

 private:
  DataBaseConnector& dbConnector;

  bool checkTableExists(const std::string& tableName);
  bool createTables();
};

#endif  // DATABASE_INITIALIZER_H
