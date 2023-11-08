#pragma once
#include "../../models/Aircraft.h"
#include "IdMapper.h"

class AircraftIdMapper : public IdMapper {
 public:
  static AircraftIdMapper& getInstance() {
    static AircraftIdMapper instance;
    return instance;
  }

  std::vector<Aircraft> aircraftVector;

 private:
  AircraftIdMapper() = default;
};