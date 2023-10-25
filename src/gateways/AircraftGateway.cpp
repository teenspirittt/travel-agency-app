#include "AircraftGateway.h"

AircraftGateway::AircraftGateway() {
  dbConnector = &DataBaseConnector::getInstance();
}

bool AircraftGateway::insertAircraft(const std::string &aircraftType,
                                     int carrierId,
                                     const std::string &manufacturer,
                                     int capacity) {
  std::string query =
      "INSERT INTO Aircraft (aircraft_type, carrier_id, manufacturer, "
      "capacity) "
      "VALUES ('" +
      aircraftType + "', " + std::to_string(carrierId) + ", '" + manufacturer +
      "', " + std::to_string(capacity) + ")";
  SqlExecuter &sqlExecuter = SqlExecuter::getInstance();
  return sqlExecuter.executeSQL(query);
}

bool AircraftGateway::deleteAircraft(int aircraftId) {
  std::string query =
      "DELETE FROM Aircraft WHERE id = " + std::to_string(aircraftId);
  SqlExecuter &sqlExecuter = SqlExecuter::getInstance();
  return sqlExecuter.executeSQL(query);
}

bool AircraftGateway::getAircraft(int aircraftId, std::string &aircraftType,
                                  int &carrierId, std::string &manufacturer,
                                  int &capacity) {
  std::string query =
      "SELECT aircraft_type, carrier_id, manufacturer, capacity FROM Aircraft "
      "WHERE id = " +
      std::to_string(aircraftId);
  SqlExecuter &sqlExecuter = SqlExecuter::getInstance();

  SQLHSTMT hstmt;

  sqlExecuter.executeSQLWithResults(query, hstmt);
  if (hstmt) {
    SQLRETURN ret = SQLFetch(hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      SQLLEN aircraftTypeLength, manufacturerLength;
      char aircraftTypeBuffer[256], manufacturerBuffer[256];

      ret = SQLGetData(hstmt, 1, SQL_C_CHAR, aircraftTypeBuffer,
                       sizeof(aircraftTypeBuffer), &aircraftTypeLength);
      ret = SQLGetData(hstmt, 2, SQL_C_LONG, &carrierId, 0, NULL);
      ret = SQLGetData(hstmt, 3, SQL_C_CHAR, manufacturerBuffer,
                       sizeof(manufacturerBuffer), &manufacturerLength);
      ret = SQLGetData(hstmt, 4, SQL_C_LONG, &capacity, 0, NULL);

      aircraftType = aircraftTypeBuffer;
      manufacturer = manufacturerBuffer;

      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return true;
    } else {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    }
  }
  return false;
}

bool AircraftGateway::updateAircraft(int aircraftId,
                                     const std::string &aircraftType,
                                     int carrierId,
                                     const std::string &manufacturer,
                                     int capacity) {
  std::string query = "UPDATE Aircraft SET aircraft_type = '" + aircraftType +
                      "', carrier_id = " + std::to_string(carrierId) +
                      ", manufacturer = '" + manufacturer +
                      "', capacity = " + std::to_string(capacity) +
                      " WHERE id = " + std::to_string(aircraftId);

  SqlExecuter &sqlExecuter = SqlExecuter::getInstance();

  return sqlExecuter.executeSQL(query);
}

bool AircraftGateway::findAircraftByType(
    const std::string &aircraftType, std::vector<int> &matchingAircraftIds) {
  std::string query =
      "SELECT id FROM Aircraft WHERE aircraft_type = '" + aircraftType + "'";

  SqlExecuter &sqlExecuter = SqlExecuter::getInstance();

  SQLHSTMT hstmt;
  if (sqlExecuter.executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret;

    while ((ret = SQLFetch(hstmt)) == SQL_SUCCESS ||
           ret == SQL_SUCCESS_WITH_INFO) {
      int aircraftId;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &aircraftId, 0, NULL);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        matchingAircraftIds.push_back(aircraftId);
      }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !matchingAircraftIds.empty();
  }
  return false;
}

bool AircraftGateway::getAllAircraft(std::vector<int> &aircraftIds) {
  std::string query = "SELECT id FROM Aircraft";

  SqlExecuter &sqlExecuter = SqlExecuter::getInstance();

  SQLHSTMT hstmt;
  if (sqlExecuter.executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret;

    while ((ret = SQLFetch(hstmt)) == SQL_SUCCESS ||
           ret == SQL_SUCCESS_WITH_INFO) {
      int aircraftId;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &aircraftId, 0, NULL);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        aircraftIds.push_back(aircraftId);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !aircraftIds.empty();
  }

  std::cout << "Failed to execute SQL or no aircraft found." << std::endl;
  return false;
}

bool AircraftGateway::findAircraftByCarrier(
    int carrierId, std::vector<int> &matchingAircraftIds) {
  std::string query =
      "SELECT id FROM Aircraft WHERE carrier_id = " + std::to_string(carrierId);

  SqlExecuter &sqlExecuter = SqlExecuter::getInstance();

  SQLHSTMT hstmt;
  if (sqlExecuter.executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret;

    while ((ret = SQLFetch(hstmt)) == SQL_SUCCESS ||
           ret == SQL_SUCCESS_WITH_INFO) {
      int aircraftId;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &aircraftId, 0, NULL);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        matchingAircraftIds.push_back(aircraftId);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

    return !matchingAircraftIds.empty();
  }

  std::cout << "Failed to execute SQL or no matching aircraft found."
            << std::endl;
  return false;
}