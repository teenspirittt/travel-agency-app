#ifndef CLIENTROUTE_H
#define CLIENTROUTE_H

class ClientRoute {
 public:
  ClientRoute(int clientId, int routeId)
      : clientId(clientId), routeId(routeId) {}

  int getClientId() const { return clientId; }
  int getRouteId() const { return routeId; }

  void setClientId(int clientId) { this->clientId = clientId; }
  void setRouteId(int routeId) { this->routeId = routeId; }

 private:
  int clientId;
  int routeId;
};

#endif
