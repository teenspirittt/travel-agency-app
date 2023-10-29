#ifndef EMPLOYEETRANSFERS_H
#define EMPLOYEETRANSFERS_H

#include <sql.h>
#include <sqlext.h>

#include <string>
#include <vector>

#include "../utils/DataBaseConnector.h"
#include "../utils/SqlExecuter.h"

class EmployeeTransfers {
 public:
  EmployeeTransfers();
  bool insertTransfer(int employeeId, const std::string &oldPosition,
                      const std::string &transferReason,
                      const std::string &orderNumber);
  bool deleteTransfer(int transferId);
  bool getTransfer(int transferId, int &employeeId, std::string &oldPosition,
                   std::string &transferReason, std::string &orderNumber);
  bool updateTransfer(int transferId, int employeeId,
                      const std::string &oldPosition,
                      const std::string &transferReason,
                      const std::string &orderNumber);
  bool getAllTransfers(
      std::vector<std::tuple<int, int, std::string, std::string, std::string,
                             std::string>> &transferData);
  bool findTransfersByEmployee(int employeeId,
                               std::vector<int> &matchingTransferIds);

 private:
  DataBaseConnector *dbConnector;
  SqlExecuter *sqlExecuter;
};

#endif
