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

bool EmployeeGateway::getAllEmployees(
    std::vector<std::tuple<int, std::string, std::string, std::string,
                           std::string, double>> &employeeData) {
  std::string query =
      "SELECT id, full_name, address, date_of_birth, position, salary FROM "
      "Employees";
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int id;
      char full_nameBuffer[256];
      char addressBuffer[256];  // Изменено с 256 на 65536 для поддержки
                                // текстового поля.
      char date_of_birthBuffer[11];  // Изменено с 10 на 11, так как длина даты
                                     // 10 символов (например, "yyyy-mm-dd").
      char positionBuffer[256];
      double salary;

      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        ret = SQLGetData(hstmt, 1, SQL_C_LONG, &id, 0, NULL);
        ret = SQLGetData(hstmt, 2, SQL_C_CHAR, full_nameBuffer,
                         sizeof(full_nameBuffer), NULL);
        ret = SQLGetData(hstmt, 3, SQL_C_CHAR, addressBuffer,
                         sizeof(addressBuffer), NULL);
        ret = SQLGetData(hstmt, 4, SQL_C_CHAR, date_of_birthBuffer,
                         sizeof(date_of_birthBuffer), NULL);
        ret = SQLGetData(hstmt, 5, SQL_C_CHAR, positionBuffer,
                         sizeof(positionBuffer), NULL);
        ret = SQLGetData(hstmt, 6, SQL_C_DOUBLE, &salary, 0, NULL);

        std::string full_name(full_nameBuffer);
        std::string address(addressBuffer);
        std::string date_of_birth(date_of_birthBuffer);
        std::string position(positionBuffer);

        employeeData.emplace_back(id, full_name, address, date_of_birth,
                                  position, salary);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !employeeData.empty();
  }

  return false;
}

int EmployeeGateway::getLastInsertedId() {
  SQLHSTMT hstmt;
  std::string query = "SELECT last_value FROM employees_id_seq";

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    int lastInsertedId = -1;

    ret = SQLFetch(hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      SQLLEN idLength;
      ret = SQLGetData(hstmt, 1, SQL_C_SLONG, &lastInsertedId, 0, &idLength);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        return lastInsertedId;
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
  }

  std::cerr << "Failed to get the last inserted ID.\n";
  return -1;
}