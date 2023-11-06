#pragma once
#include <vector>

#include "../models/EmployeeTransfers.h"
#include "IdMapper.h"

class EmployeeTransfersIdMapper : public IdMapper {
 public:
  static EmployeeTransfersIdMapper& getInstance() {
    static EmployeeTransfersIdMapper instance;
    return instance;
  }

  std::vector<EmployeeTransfers> employeeTransfersVector;

 private:
  EmployeeTransfersIdMapper() = default;
};