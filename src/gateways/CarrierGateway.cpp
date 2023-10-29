#include "CarrierGateway.h"

CarrierGateway::CarrierGateway() {
  dbConnector = &DataBaseConnector::getInstance();
  sqlExecuter = &SqlExecuter::getInstance();
}

bool CarrierGateway::insertCarrier(const std::string &name) {
  std::string query = "INSERT INTO Carriers (name) VALUES ('" + name + "')";
  return sqlExecuter->executeSQL(query);
}

bool CarrierGateway::deleteCarrier(int carrierId) {
  std::string query =
      "DELETE FROM Carriers WHERE id = " + std::to_string(carrierId);
  return sqlExecuter->executeSQL(query);
}

bool CarrierGateway::getCarrier(int carrierId, std::string &name) {
  std::string query =
      "SELECT name FROM Carriers WHERE id = " + std::to_string(carrierId);
  SQLHSTMT hstmt;
  char nameBuffer[256];
  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQLFetch(hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      SQLLEN nameLength;
      ret = SQLGetData(hstmt, 1, SQL_C_CHAR, nameBuffer, sizeof(nameBuffer),
                       &nameLength);

      name = nameBuffer;
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        return true;
      }
    }
  }

  return false;
}

bool CarrierGateway::updateCarrier(int carrierId, const std::string &name) {
  std::string query = "UPDATE Carriers SET name = '" + name +
                      "' WHERE id = " + std::to_string(carrierId);
  return sqlExecuter->executeSQL(query);
}

bool CarrierGateway::getAllCarriers(
    std::vector<std::tuple<int, std::string>> &carrierData) {
  std::string query = "SELECT id, name FROM Carriers";
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int id;
      char nameBuffer[256];

      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        ret = SQLGetData(hstmt, 1, SQL_C_LONG, &id, 0, NULL);
        ret = SQLGetData(hstmt, 2, SQL_C_CHAR, nameBuffer, sizeof(nameBuffer),
                         NULL);

        std::string name(nameBuffer);
        carrierData.emplace_back(id, name);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !carrierData.empty();
  }

  return false;
}
