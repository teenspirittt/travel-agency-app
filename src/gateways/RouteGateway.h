#ifndef ROUTEGATEWAY_H
#define ROUTEGATEWAY_H

#include <sql.h>
#include <sqlext.h>

#include <string>
#include <vector>

#include "../utils/DataBaseConnector.h"
#include "../utils/SqlExecuter.h"

class RouteGateway {
 public:
  RouteGateway();
  bool insertRoute(const std::string &name, const std::string &country,
                   const std::string &city, int duration, int hotelId,
                   int flightId, int employeeId,
                   const std::string &agencyRepName,
                   const std::string &agencyRepPhone);
  bool deleteRoute(int routeId);
  bool getRoute(int routeId, std::string &name, std::string &country,
                std::string &city, int &duration, int &hotelId, int &flightId,
                int &employeeId, std::string &agencyRepName,
                std::string &agencyRepPhone);
  bool updateRoute(int routeId, const std::string &name,
                   const std::string &country, const std::string &city,
                   int duration, int hotelId, int flightId, int employeeId,
                   const std::string &agencyRepName,
                   const std::string &agencyRepPhone);
  bool getAllRoutes(std::vector<int> &routeIds);
  bool findRoutesByHotel(int hotelId, std::vector<int> &matchingRouteIds);
  bool findRoutesByEmployee(int employeeId, std::vector<int> &matchingRouteIds);
  bool findRoutesByCity(const std::string &city,
                        std::vector<int> &matchingRouteIds);

 private:
  DataBaseConnector *dbConnector;
  SqlExecuter *sqlExecuter;
};

#endif
