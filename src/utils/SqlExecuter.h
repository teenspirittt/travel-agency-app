#ifndef SQLEXECUTER_H
#define SQLEXECUTER_H

#include <sql.h>

#include <vector>

#include "DataBaseConnector.h"

class SqlExecuter {
 public:
  static SqlExecuter& getInstance() {
    static SqlExecuter instance;
    return instance;
  }

  bool executeSQL(const std::string& sqlQuery);

  bool executeSQLWithResults(const std::string& sqlQuery, SQLHSTMT& hstmt);

 private:
  SqlExecuter();
  DataBaseConnector* dbConnector;
};

#endif  // SQLEXECUTER_H
