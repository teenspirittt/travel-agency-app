#include "EmployeeGateway.h"

EmployeeGateway::EmployeeGateway() {
  dbConnector = &DataBaseConnector::getInstance();
  sqlExecuter = &SqlExecuter::getInstance();
}

bool EmployeeGateway::insertEmployee(const std::string &fullName,
                                     const std::string &address,
                                     const std::string &dateOfBirth,
                                     const std::string &position,
                                     double salary) {
  std::string query =
      "INSERT INTO Employees (full_name, address, date_of_birth, position, "
      "salary) "
      "VALUES ('" +
      fullName + "', '" + address + "', '" + dateOfBirth + "', '" + position +
      "', " + std::to_string(salary) + ")";

  return sqlExecuter->executeSQL(query);
}

bool EmployeeGateway::deleteEmployee(int employeeId) {
  std::string query =
      "DELETE FROM Employees WHERE id = " + std::to_string(employeeId);

  return sqlExecuter->executeSQL(query);
}

bool EmployeeGateway::getEmployee(int employeeId, std::string &fullName,
                                  std::string &address,
                                  std::string &dateOfBirth,
                                  std::string &position, double &salary) {
  std::string query =
      "SELECT full_name, address, date_of_birth, position, salary FROM "
      "Employees "
      "WHERE id = " +
      std::to_string(employeeId);

  SQLHSTMT hstmt;
  char fullNameBuffer[256];
  char addressBuffer[256];
  char dateOfBirthBuffer[256];
  char positionBuffer[256];

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    ret = SQLFetch(hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      SQLLEN fullNameLength, addressLength, dateOfBirthLength, positionLength;
      ret = SQLGetData(hstmt, 1, SQL_C_CHAR, fullNameBuffer,
                       sizeof(fullNameBuffer), &fullNameLength);
      ret = SQLGetData(hstmt, 2, SQL_C_CHAR, addressBuffer,
                       sizeof(addressBuffer), &addressLength);
      ret = SQLGetData(hstmt, 3, SQL_C_CHAR, dateOfBirthBuffer,
                       sizeof(dateOfBirthBuffer), &dateOfBirthLength);
      ret = SQLGetData(hstmt, 4, SQL_C_CHAR, positionBuffer,
                       sizeof(positionBuffer), &positionLength);
      ret = SQLGetData(hstmt, 5, SQL_C_DOUBLE, &salary, 0, NULL);

      fullName = fullNameBuffer;
      address = addressBuffer;
      dateOfBirth = dateOfBirthBuffer;
      position = positionBuffer;

      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return true;
    } else {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    }
  }

  return false;
}

bool EmployeeGateway::updateEmployee(int employeeId,
                                     const std::string &fullName,
                                     const std::string &address,
                                     const std::string &dateOfBirth,
                                     const std::string &position,
                                     double salary) {
  std::string query =
      "UPDATE Employees "
      "SET full_name = '" +
      fullName + "', address = '" + address + "', date_of_birth = '" +
      dateOfBirth + "', position = '" + position +
      "', salary = " + std::to_string(salary) +
      " WHERE id = " + std::to_string(employeeId);

  return sqlExecuter->executeSQL(query);
}

bool EmployeeGateway::findEmployeesByPosition(
    const std::string &position, std::vector<int> &matchingEmployeeIds) {
  std::string query =
      "SELECT id FROM employees WHERE position = '" + position + "'";

  SQLHSTMT hstmt;
  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int flightId;
      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        ret = SQLGetData(hstmt, 1, SQL_C_LONG, &flightId, 0, NULL);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
          matchingEmployeeIds.push_back(flightId);
        }
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !matchingEmployeeIds.empty();
  }

  return false;
}