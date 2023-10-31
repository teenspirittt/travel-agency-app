#ifndef CLIENTROUTEGATEWAY_H
#define CLIENTROUTEGATEWAY_H

#include <sql.h>
#include <sqlext.h>

#include <string>
#include <tuple>
#include <vector>

#include "../utils/DataBaseConnector.h"
#include "../utils/SqlExecuter.h"
class ClientRouteGateway {
 public:
  ClientRouteGateway();
  bool insertClientRoute(int clientId, int routeId);
  bool deleteClientRoute(int clientId, int routeId);
  bool getClientRoute(int clientId, int routeId);
  bool getAllClientRoutes(std::vector<std::pair<int, int>> &clientRoutePairs);
  bool findRoutesByClient(int clientId, std::vector<int> &matchingRouteIds);

 private:
  DataBaseConnector *dbConnector;
  SqlExecuter *sqlExecuter;
};

#endif
