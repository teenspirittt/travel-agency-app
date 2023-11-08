#ifndef RPBD_EMPLOYEEIDMAPPER_H
#define RPBD_EMPLOYEEIDMAPPER_H

#include <vector>

#include "../../models/Employees.h"
#include "IdMapper.h"

class EmployeeIdMapper : public IdMapper {
 public:
  static EmployeeIdMapper &getInstance() {
    static EmployeeIdMapper instance;
    return instance;
  }

  std::vector<Employees> employeeVector;

 private:
  EmployeeIdMapper() = default;
};

#endif  // RPBD_EMPLOYEEIDMAPPER_H
