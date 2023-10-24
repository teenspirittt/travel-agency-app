#include "AircraftGateway.h"

AircraftGateway::AircraftGateway(SQLHDBC dbc) : hdbc(dbc) {}

bool AircraftGateway::insertAircraft(const std::string &aircraftType,
                                     int carrierId,
                                     const std::string &manufacturer,
                                     int capacity) {
  SQLHSTMT hstmt;
  SQLRETURN ret;

  std::string query =
      "INSERT INTO aircraft (aircraft_type, carrier_id, manufacturer, "
      "capacity) VALUES (?, ?, ?, ?)";

  ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    return false;
  }

  ret = SQLPrepare(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  SQLCHAR aircraftTypeParam[256];
  SQLLEN aircraftTypeInd = SQL_NTS;
  std::string aircraftTypeStr = aircraftType;
  strcpy((char *)aircraftTypeParam, aircraftTypeStr.c_str());

  ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         256, 0, aircraftTypeParam, 0, &aircraftTypeInd);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0,
                         0, &carrierId, 0, NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  SQLCHAR manufacturerParam[256];
  SQLLEN manufacturerInd = SQL_NTS;
  std::string manufacturerStr = manufacturer;
  strcpy((char *)manufacturerParam, manufacturerStr.c_str());

  ret = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         256, 0, manufacturerParam, 0, &manufacturerInd);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0,
                         0, &capacity, 0, NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecute(hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    return false;
  }

  return true;
}

bool AircraftGateway::deleteAircraft(int aircraftId) {
  SQLHSTMT hstmt;
  SQLRETURN ret;

  std::string query = "DELETE FROM Aircraft WHERE id = ?";

  ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    return false;
  }

  ret = SQLPrepare(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0,
                         0, &aircraftId, 0, NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecute(hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    return false;
  }

  return true;
}

bool AircraftGateway::getAircraft(int aircraftId, std::string &aircraftType,
                                  int &carrierId, std::string &manufacturer,
                                  int &capacity) {
  SQLHSTMT hstmt;
  SQLRETURN ret;

  std::string query =
      "SELECT aircraft_type, carrier_id, manufacturer, capacity FROM Aircraft "
      "WHERE id = ?";

  ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    return false;
  }

  ret = SQLPrepare(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0,
                         0, &aircraftId, 0, NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecute(hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindCol(hstmt, 1, SQL_C_CHAR, &aircraftType, sizeof(aircraftType),
                   NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindCol(hstmt, 2, SQL_C_LONG, &carrierId, sizeof(carrierId), NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindCol(hstmt, 3, SQL_C_CHAR, &manufacturer, sizeof(manufacturer),
                   NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindCol(hstmt, 4, SQL_C_LONG, &capacity, sizeof(capacity), NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLFetch(hstmt);
  if (ret == SQL_NO_DATA) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;  // Не найдено записей с указанным ID
  } else if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
  return true;
}

bool AircraftGateway::updateAircraft(int aircraftId,
                                     const std::string &aircraftType,
                                     int carrierId,
                                     const std::string &manufacturer,
                                     int capacity) {
  SQLHSTMT hstmt;
  SQLRETURN ret;

  std::string query =
      "UPDATE Aircraft SET aircraft_type = ?, carrier_id = ?, manufacturer = "
      "?, capacity = ? WHERE id = ?";

  ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    return false;
  }

  ret = SQLPrepare(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0,
                         0, (SQLPOINTER)aircraftType.c_str(), 0, NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0,
                         0, &carrierId, 0, NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0,
                         0, (SQLPOINTER)manufacturer.c_str(), 0, NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0,
                         0, &capacity, 0, NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0,
                         0, &aircraftId, 0, NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecute(hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
  return true;
}

bool AircraftGateway::getAllAircraft(std::vector<int> &aircraftIds) {
  SQLHSTMT hstmt;
  SQLRETURN ret;

  std::string query = "SELECT id FROM Aircraft";

  ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    return false;
  }

  ret = SQLPrepare(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecute(hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  SQLINTEGER aircraftId;
  SQLLEN indicator;

  while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA) {
    if (ret == SQL_ERROR || ret == SQL_SUCCESS_WITH_INFO) {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return false;
    }

    ret = SQLGetData(hstmt, 1, SQL_C_LONG, &aircraftId, 0, &indicator);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return false;
    }

    if (indicator == SQL_NULL_DATA) {
      continue;
    }

    aircraftIds.push_back(aircraftId);
  }

  SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
  return true;
}

bool AircraftGateway::findAircraftByType(
    const std::string &aircraftType, std::vector<int> &matchingAircraftIds) {
  SQLHSTMT hstmt;
  SQLRETURN ret;

  std::string query = "SELECT id FROM Aircraft WHERE aircraft_type = ?";

  ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    return false;
  }

  ret = SQLPrepare(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0,
                         0, (SQLPOINTER)aircraftType.c_str(), 0, NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecute(hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  SQLINTEGER aircraftId;
  SQLLEN indicator;

  while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA) {
    if (ret == SQL_ERROR || ret == SQL_SUCCESS_WITH_INFO) {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return false;
    }

    ret = SQLGetData(hstmt, 1, SQL_C_LONG, &aircraftId, 0, &indicator);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return false;
    }

    if (indicator == SQL_NULL_DATA) {
      // Нулевые данные
      continue;
    }

    matchingAircraftIds.push_back(aircraftId);
  }

  SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
  return true;
}

bool AircraftGateway::findAircraftByCarrier(
    int carrierId, std::vector<int> &matchingAircraftIds) {
  SQLHSTMT hstmt;
  SQLRETURN ret;

  std::string query = "SELECT id FROM Aircraft WHERE carrier_id = ?";

  ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    return false;
  }

  ret = SQLPrepare(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0,
                         0, &carrierId, 0, NULL);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecute(hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  SQLINTEGER aircraftId;
  SQLLEN indicator;

  while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA) {
    if (ret == SQL_ERROR || ret == SQL_SUCCESS_WITH_INFO) {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return false;
    }

    ret = SQLGetData(hstmt, 1, SQL_C_LONG, &aircraftId, 0, &indicator);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return false;
    }

    if (indicator == SQL_NULL_DATA) {
      // Нулевые данные
      continue;
    }

    matchingAircraftIds.push_back(aircraftId);
  }

  SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
  return true;
}
