#include "SqlExecuter.h"

#include <sql.h>
#include <sqlext.h>

#include <string>
#include <vector>

SqlExecuter::SqlExecuter() { dbConnector = &DataBaseConnector::getInstance(); }

bool SqlExecuter::executeSQL(const std::string& sqlQuery) {
  SQLRETURN ret;
  SQLHSTMT hstmt;

  ret = SQLAllocHandle(SQL_HANDLE_STMT, dbConnector->getConnection(), &hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    std::cerr << "Failed to allocate statement handle." << std::endl;
    return false;
  }

  ret = SQLExecDirect(hstmt, (SQLCHAR*)sqlQuery.c_str(), SQL_NTS);

  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    std::cerr << "SQL query execution failed." << std::endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

    return true;
}

bool SqlExecuter::executeSQLWithResults(const std::string& sqlQuery,
                                        SQLHSTMT& hstmt) {
  SQLRETURN ret;

  ret = SQLAllocHandle(SQL_HANDLE_STMT, dbConnector->getConnection(), &hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    std::cerr << "Failed to allocate statement handle." << std::endl;
    return false;
  }

  ret = SQLExecDirect(hstmt, (SQLCHAR*)sqlQuery.c_str(), SQL_NTS);

  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    std::cerr << "SQL query execution failed." << std::endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  return true;
}