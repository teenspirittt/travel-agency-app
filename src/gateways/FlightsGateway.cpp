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

bool FlightsGateway::getAllFlights(
    std::vector<std::tuple<int, std::string, std::string, int, std::string, int,
                           int>> &flightData) {
  std::string query =
      "SELECT id, flight_number, departure_date, aircraft_id, class, "
      "available_seats, carrier_id FROM Flights";
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int id, aircraft_id, available_seats, carrier_id;
      char flight_numberBuffer[51];  // Изменено с 50 на 51 для поддержки полей
                                     // в 50 символов.
      char departure_dateBuffer[20];  // Изменено с 19 на 20, так как длина
                                      // timestamp 19 символов.
      char classBuffer[51];  // Изменено с 50 на 51 для поддержки полей в 50
                             // символов.

      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        ret = SQLGetData(hstmt, 1, SQL_C_LONG, &id, 0, NULL);
        ret = SQLGetData(hstmt, 2, SQL_C_CHAR, flight_numberBuffer,
                         sizeof(flight_numberBuffer), NULL);
        ret = SQLGetData(hstmt, 3, SQL_C_CHAR, departure_dateBuffer,
                         sizeof(departure_dateBuffer), NULL);
        ret = SQLGetData(hstmt, 4, SQL_C_LONG, &aircraft_id, 0, NULL);
        ret = SQLGetData(hstmt, 5, SQL_C_CHAR, classBuffer, sizeof(classBuffer),
                         NULL);
        ret = SQLGetData(hstmt, 6, SQL_C_LONG, &available_seats, 0, NULL);
        ret = SQLGetData(hstmt, 7, SQL_C_LONG, &carrier_id, 0, NULL);

        std::string flight_number(flight_numberBuffer);
        std::string departure_date(departure_dateBuffer);
        std::string flight_class(classBuffer);

        flightData.emplace_back(id, flight_number, departure_date, aircraft_id,
                                flight_class, available_seats, carrier_id);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !flightData.empty();
  }

  return false;
}
