#ifndef RPBD_ROUTEIDMAPPER_H
#define RPBD_ROUTEIDMAPPER_H

#include <vector>

#include "../../models/Route.h"
#include "IdMapper.h"

class RouteIdMapper : public IdMapper {
 public:
  static RouteIdMapper &getInstance() {
    static RouteIdMapper instance;
    return instance;
  }

  std::vector<Route> routeVector;

 private:
  RouteIdMapper() = default;
};

#endif  // RPBD_ROUTEIDMAPPER_H
