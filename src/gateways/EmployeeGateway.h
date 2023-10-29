#ifndef EMPLOYEEGATEWAY_H
#define EMPLOYEEGATEWAY_H

#include <sql.h>
#include <sqlext.h>

#include <string>
#include <vector>

#include "../utils/DataBaseConnector.h"
#include "../utils/SqlExecuter.h"

class EmployeeGateway {
 public:
  EmployeeGateway();

  bool insertEmployee(const std::string &fullName, const std::string &address,
                      const std::string &dateOfBirth,
                      const std::string &position, double salary);
  bool deleteEmployee(int employeeId);
  bool getEmployee(int employeeId, std::string &fullName, std::string &address,
                   std::string &dateOfBirth, std::string &position,
                   double &salary);
  bool updateEmployee(int employeeId, const std::string &fullName,
                      const std::string &address,
                      const std::string &dateOfBirth,
                      const std::string &position, double salary);
  bool getAllEmployees(
      std::vector<std::tuple<int, std::string, std::string, std::string,
                             std::string, double>> &employeeData);
  bool findEmployeesByPosition(const std::string &position,
                               std::vector<int> &matchingEmployeeIds);

 private:
  DataBaseConnector *dbConnector;
  SqlExecuter *sqlExecuter;
};

#endif
