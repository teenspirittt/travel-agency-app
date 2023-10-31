#pragma once
#include "IdMapper.h"

class CarrierIdMapper : public IdMapper {
 public:
  static CarrierIdMapper& getInstance() {
    static CarrierIdMapper instance;
    return instance;
  }

 private:
  CarrierIdMapper() = default;
};