#ifndef RPBD_FLIGHTIDMAPPER_H
#define RPBD_FLIGHTIDMAPPER_H

#include <vector>

#include "../models/Flights.h"
#include "IdMapper.h"

class FlightIdMapper : public IdMapper {
 public:
  static FlightIdMapper &getInstance() {
    static FlightIdMapper instance;
    return instance;
  }

  std::vector<Flights> flightVector;

 private:
  FlightIdMapper() = default;
};

#endif  // RPBD_FLIGHTIDMAPPER_H
