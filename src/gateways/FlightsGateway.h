#ifndef FLIGHTSGATEWAY_H
#define FLIGHTSGATEWAY_H

#include <sql.h>
#include <sqlext.h>

#include <string>
#include <vector>

#include "../utils/DataBaseConnector.h"
#include "../utils/SqlExecuter.h"

class FlightsGateway {
 public:
  FlightsGateway();

  bool insertFlight(const std::string &flightNumber,
                    const std::string &departureDate, int aircraftId,
                    const std::string &flightClass, int availableSeats,
                    int carrierId);

  bool deleteFlight(int flightId);

  bool getFlight(int flightId, std::string &flightNumber,
                 std::string &departureDate, int &aircraftId,
                 std::string &flightClass, int &availableSeats, int &carrierId);

  bool updateFlight(int flightId, const std::string &flightNumber,
                    const std::string &departureDate, int aircraftId,
                    const std::string &flightClass, int availableSeats,
                    int carrierId);

  bool getAllFlights(
      std::vector<std::tuple<int, std::string, std::string, int, std::string,
                             int, int>> &flightData);

  bool findFlightsByNumber(const std::string &flightNumber,
                           std::vector<int> &matchingFlightIds);

  bool findFlightsByAircraft(int aircraftId,
                             std::vector<int> &matchingFlightIds);

 private:
  DataBaseConnector *dbConnector;
  SqlExecuter *sqlExecuter;
};

#endif
