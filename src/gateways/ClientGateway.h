#ifndef CLIENTGATEWAY_H
#define CLIENTGATEWAY_H

#include <sql.h>
#include <sqlext.h>

#include <string>
#include <tuple>
#include <vector>

#include "../utils/DataBaseConnector.h"
#include "../utils/SqlExecuter.h"
class ClientGateway {
 public:
  ClientGateway();

  bool insertClient(const std::string &fullName, const std::string &phone,
                    const std::string &orderDate, const std::string &seat);

  bool deleteClient(int clientId);

  bool getClient(int clientId, std::string &fullName, std::string &phone,
                 std::string &orderDate, std::string &seat);

  bool updateClient(int clientId, const std::string &fullName,
                    const std::string &phone, const std::string &orderDate,
                    const std::string &seat);

  bool getAllClients(
      std::vector<std::tuple<int, std::string, std::string, std::string,
                             std::string>> &clientData);

  bool findClientsByFullName(const std::string &fullName,
                             std::vector<int> &matchingClientIds);

  bool findClientsByOrderDate(const std::string &orderDate,
                              std::vector<int> &matchingClientIds);

  int getLastInsertedId();

 private:
  DataBaseConnector *dbConnector;
  SqlExecuter *sqlExecuter;
};

#endif
