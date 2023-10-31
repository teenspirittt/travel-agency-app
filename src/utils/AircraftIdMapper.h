#pragma once
#include "IdMapper.h"

class AircraftIdMapper : public IdMapper {
 public:
  static AircraftIdMapper& getInstance() {
    static AircraftIdMapper instance;
    return instance;
  }

 private:
  AircraftIdMapper() = default;
};