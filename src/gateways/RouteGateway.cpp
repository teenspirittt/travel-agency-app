#include "RouteGateway.h"

RouteGateway::RouteGateway() {
  dbConnector = &DataBaseConnector::getInstance();
  sqlExecuter = &SqlExecuter::getInstance();
}

bool RouteGateway::insertRoute(const std::string &name,
                               const std::string &country,
                               const std::string &city, int duration,
                               int hotelId, int flightId, int employeeId,
                               const std::string &agencyRepName,
                               const std::string &agencyRepPhone) {
  std::string query =
      "INSERT INTO Route (name, country, city, duration, hotel_id, "
      "flight_id, employee_id, agency_representative_name, "
      "agency_representative_phone) "
      "VALUES ('" +
      name + "', '" + country + "', '" + city + "', " +
      std::to_string(duration) + ", " + std::to_string(hotelId) + ", " +
      std::to_string(flightId) + ", " + std::to_string(employeeId) + ", '" +
      agencyRepName + "', '" + agencyRepPhone + "')";

  return sqlExecuter->executeSQL(query);
}

bool RouteGateway::deleteRoute(int routeId) {
  std::string query = "DELETE FROM Route WHERE id = " + std::to_string(routeId);
  return sqlExecuter->executeSQL(query);
}

bool RouteGateway::updateRoute(int routeId, const std::string &name,
                               const std::string &country,
                               const std::string &city, int duration,
                               int hotelId, int flightId, int employeeId,
                               const std::string &agencyRepName,
                               const std::string &agencyRepPhone) {
  std::string query = "UPDATE Route SET name = '" + name + "', country = '" +
                      country + "', city = '" + city +
                      "', duration = " + std::to_string(duration) +
                      ", hotel_id = " + std::to_string(hotelId) +
                      ", flight_id = " + std::to_string(flightId) +
                      ", employee_id = " + std::to_string(employeeId) +
                      ", agency_representative_name = '" + agencyRepName +
                      "', agency_representative_phone = '" + agencyRepPhone +
                      "' WHERE id = " + std::to_string(routeId);

  return sqlExecuter->executeSQL(query);
}

bool RouteGateway::getAllRoutes(
    std::vector<std::tuple<int, std::string, std::string, std::string, int, int,
                           int, int, std::string, std::string>> &routeData) {
  std::string query =
      "SELECT id, name, country, city, duration, hotel_id, flight_id, "
      "employee_id, "
      "agency_representative_name, agency_representative_phone FROM Routes";
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int id, duration, hotel_id, flight_id, employee_id;
      char nameBuffer[256];
      char countryBuffer[256];
      char cityBuffer[256];
      char agency_representative_nameBuffer[256];
      char agency_representative_phoneBuffer[256];

      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        ret = SQLGetData(hstmt, 1, SQL_C_LONG, &id, 0, NULL);
        ret = SQLGetData(hstmt, 2, SQL_C_CHAR, nameBuffer, sizeof(nameBuffer),
                         NULL);
        ret = SQLGetData(hstmt, 3, SQL_C_CHAR, countryBuffer,
                         sizeof(countryBuffer), NULL);
        ret = SQLGetData(hstmt, 4, SQL_C_CHAR, cityBuffer, sizeof(cityBuffer),
                         NULL);
        ret = SQLGetData(hstmt, 5, SQL_C_LONG, &duration, 0, NULL);
        ret = SQLGetData(hstmt, 6, SQL_C_LONG, &hotel_id, 0, NULL);
        ret = SQLGetData(hstmt, 7, SQL_C_LONG, &flight_id, 0, NULL);
        ret = SQLGetData(hstmt, 8, SQL_C_LONG, &employee_id, 0, NULL);
        ret = SQLGetData(hstmt, 9, SQL_C_CHAR, agency_representative_nameBuffer,
                         sizeof(agency_representative_nameBuffer), NULL);
        ret =
            SQLGetData(hstmt, 10, SQL_C_CHAR, agency_representative_phoneBuffer,
                       sizeof(agency_representative_phoneBuffer), NULL);

        std::string name(nameBuffer);
        std::string country(countryBuffer);
        std::string city(cityBuffer);
        std::string agency_representative_name(
            agency_representative_nameBuffer);
        std::string agency_representative_phone(
            agency_representative_phoneBuffer);

        routeData.emplace_back(
            id, name, country, city, duration, hotel_id, flight_id, employee_id,
            agency_representative_name, agency_representative_phone);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !routeData.empty();
  }

  return false;
}

bool RouteGateway::getRoute(int routeId, std::string &name,
                            std::string &country, std::string &city,
                            int &duration, int &hotelId, int &flightId,
                            int &employeeId, std::string &agencyRepName,
                            std::string &agencyRepPhone) {
  std::string query =
      "SELECT name, country, city, duration, hotel_id, flight_id, employee_id, "
      "agency_representative_name, agency_representative_phone "
      "FROM Route WHERE id = " +
      std::to_string(routeId);

  SQLHSTMT hstmt;
  char nameBuffer[256];
  char countryBuffer[256];
  char cityBuffer[256];
  char agencyRepNameBuffer[256];
  char agencyRepPhoneBuffer[256];

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    ret = SQLFetch(hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      SQLLEN nameLength, countryLength, cityLength, agencyRepNameLength,
          agencyRepPhoneLength;
      ret = SQLGetData(hstmt, 1, SQL_C_CHAR, nameBuffer, sizeof(nameBuffer),
                       &nameLength);
      ret = SQLGetData(hstmt, 2, SQL_C_CHAR, countryBuffer,
                       sizeof(countryBuffer), &countryLength);
      ret = SQLGetData(hstmt, 3, SQL_C_CHAR, cityBuffer, sizeof(cityBuffer),
                       &cityLength);
      ret = SQLGetData(hstmt, 4, SQL_C_LONG, &duration, 0, NULL);
      ret = SQLGetData(hstmt, 5, SQL_C_LONG, &hotelId, 0, NULL);
      ret = SQLGetData(hstmt, 6, SQL_C_LONG, &flightId, 0, NULL);
      ret = SQLGetData(hstmt, 7, SQL_C_LONG, &employeeId, 0, NULL);
      ret = SQLGetData(hstmt, 8, SQL_C_CHAR, agencyRepNameBuffer,
                       sizeof(agencyRepNameBuffer), &agencyRepNameLength);
      ret = SQLGetData(hstmt, 9, SQL_C_CHAR, agencyRepPhoneBuffer,
                       sizeof(agencyRepPhoneBuffer), &agencyRepPhoneLength);

      name = nameBuffer;
      country = countryBuffer;
      city = cityBuffer;
      agencyRepName = agencyRepNameBuffer;
      agencyRepPhone = agencyRepPhoneBuffer;

      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return true;
    } else {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    }
  }

  std::cerr << "Failed to execute SQL or no route found." << std::endl;
  return false;
}

bool RouteGateway::findRoutesByHotel(int hotelId,
                                     std::vector<int> &matchingRouteIds) {
  std::string query =
      "SELECT id FROM Route WHERE hotel_id = " + std::to_string(hotelId);
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int routeId;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &routeId, 0, NULL);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        matchingRouteIds.push_back(routeId);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !matchingRouteIds.empty();
  }
  std::cerr << "Failed to execute SQL or no matching routes found."
            << std::endl;
  return false;
}

bool RouteGateway::findRoutesByEmployee(int employeeId,
                                        std::vector<int> &matchingRouteIds) {
  std::string query =
      "SELECT id FROM Route WHERE employee_id = " + std::to_string(employeeId);
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int routeId;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &routeId, 0, NULL);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        matchingRouteIds.push_back(routeId);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !matchingRouteIds.empty();
  }
  std::cerr << "Failed to execute SQL or no matching routes found."
            << std::endl;
  return false;
}

bool RouteGateway::findRoutesByCity(const std::string &city,
                                    std::vector<int> &matchingRouteIds) {
  std::string query = "SELECT id FROM Route WHERE city = '" + city + "'";
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int routeId;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &routeId, 0, NULL);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        matchingRouteIds.push_back(routeId);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !matchingRouteIds.empty();
  }
  std::cerr << "Failed to execute SQL or no matching routes found."
            << std::endl;
  return false;
}
