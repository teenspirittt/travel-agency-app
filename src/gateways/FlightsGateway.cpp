#include "FlightsGateway.h"

FlightsGateway::FlightsGateway() {
  dbConnector = &DataBaseConnector::getInstance();
  sqlExecuter = &SqlExecuter::getInstance();
}

bool FlightsGateway::insertFlight(const std::string &flightNumber,
                                  const std::string &departureDate,
                                  int aircraftId,
                                  const std::string &flightClass,
                                  int availableSeats, int carrierId) {
  std::string query =
      "INSERT INTO Flights (flight_number, departure_date, aircraft_id, "
      "class, available_seats, carrier_id) "
      "VALUES ('" +
      flightNumber + "', '" + departureDate + "', " +
      std::to_string(aircraftId) + ", '" + flightClass + "', " +
      std::to_string(availableSeats) + ", " + std::to_string(carrierId) + ")";

  return sqlExecuter->executeSQL(query);
}

bool FlightsGateway::deleteFlight(int flightId) {
  std::string query =
      "DELETE FROM Flights WHERE id = " + std::to_string(flightId);

  return sqlExecuter->executeSQL(query);
}

bool FlightsGateway::getFlight(int flightId, std::string &flightNumber,
                               std::string &departureDate, int &aircraftId,
                               std::string &flightClass, int &availableSeats,
                               int &carrierId) {
  std::string query =
      "SELECT flight_number, departure_date, aircraft_id, class, "
      "available_seats, carrier_id "
      "FROM Flights WHERE id = " +
      std::to_string(flightId);

  SQLHSTMT hstmt;
  char departureDateBuffer[256];
  char flightClassBuffer[256];
  char flightNumberBuffer[256];

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    ret = SQLFetch(hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      SQLLEN flightNumberLength, departureDateLength, flightClassLength;
      ret = SQLGetData(hstmt, 1, SQL_C_CHAR, flightNumberBuffer,
                       sizeof(flightNumberBuffer), &flightNumberLength);

      ret = SQLGetData(hstmt, 2, SQL_C_CHAR, departureDateBuffer,
                       sizeof(departureDateBuffer), &departureDateLength);

      ret = SQLGetData(hstmt, 3, SQL_C_LONG, &aircraftId, 0, NULL);

      ret = SQLGetData(hstmt, 4, SQL_C_CHAR, flightClassBuffer,
                       sizeof(flightClassBuffer), &flightClassLength);
      ret = SQLGetData(hstmt, 5, SQL_C_LONG, &availableSeats, 0, NULL);

      ret = SQLGetData(hstmt, 6, SQL_C_LONG, &carrierId, 0, NULL);

      departureDate = departureDateBuffer;
      flightClass = flightClassBuffer;
      flightNumber = flightNumberBuffer;

      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return true;
    } else {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    }
  }

  return false;
}

bool FlightsGateway::updateFlight(int flightId, const std::string &flightNumber,
                                  const std::string &departureDate,
                                  int aircraftId,
                                  const std::string &flightClass,
                                  int availableSeats, int carrierId) {
  std::string query = "UPDATE Flights SET flight_number = '" + flightNumber +
                      "', departure_date = '" + departureDate +
                      "', aircraft_id = " + std::to_string(aircraftId) +
                      ", class = '" + flightClass +
                      "', available_seats = " + std::to_string(availableSeats) +
                      ", carrier_id = " + std::to_string(carrierId) +
                      " WHERE id = " + std::to_string(flightId);

  return sqlExecuter->executeSQL(query);
}

bool FlightsGateway::findFlightsByNumber(const std::string &flightNumber,
                                         std::vector<int> &matchingFlightIds) {
  std::string query =
      "SELECT id FROM Flights WHERE flight_number = '" + flightNumber + "'";

  SQLHSTMT hstmt;
  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int flightId;
      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        ret = SQLGetData(hstmt, 1, SQL_C_LONG, &flightId, 0, NULL);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
          matchingFlightIds.push_back(flightId);
        }
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !matchingFlightIds.empty();
  }

  return false;
}

bool FlightsGateway::findFlightsByAircraft(
    int aircraftId, std::vector<int> &matchingFlightIds) {
  std::string query = "SELECT id FROM Flights WHERE aircraft_id = " +
                      std::to_string(aircraftId);

  SQLHSTMT hstmt;
  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int flightId;
      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        ret = SQLGetData(hstmt, 1, SQL_C_LONG, &flightId, 0, NULL);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
          matchingFlightIds.push_back(flightId);
        }
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !matchingFlightIds.empty();
  }

  return false;
}

bool FlightsGateway::getAllFlights(std::vector<int> &flightIds) {
  std::string query = "SELECT id FROM Flights";

  SqlExecuter &sqlExecuter = SqlExecuter::getInstance();

  SQLHSTMT hstmt;
  if (sqlExecuter.executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret;

    while ((ret = SQLFetch(hstmt)) == SQL_SUCCESS ||
           ret == SQL_SUCCESS_WITH_INFO) {
      int flightId;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &flightId, 0, NULL);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        flightIds.push_back(flightId);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !flightIds.empty();
  }

  std::cout << "Failed to execute SQL or no flights found." << std::endl;
  return false;
}