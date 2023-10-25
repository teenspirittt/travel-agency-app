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