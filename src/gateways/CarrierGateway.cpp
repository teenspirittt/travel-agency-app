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

bool CarrierGateway::getAllCarriers(std::vector<int> &carrierIds) {
  std::string query = "SELECT id FROM carriers";

  SqlExecuter &sqlExecuter = SqlExecuter::getInstance();

  SQLHSTMT hstmt;
  if (sqlExecuter.executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret;
    while ((ret = SQLFetch(hstmt)) == SQL_SUCCESS ||
           ret == SQL_SUCCESS_WITH_INFO) {
      int aircraftId;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &aircraftId, 0, NULL);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        carrierIds.push_back(aircraftId);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !carrierIds.empty();
  }

  std::cout << "Failed to execute SQL or no carrier found." << std::endl;
  return false;
}