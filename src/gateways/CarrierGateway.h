#ifndef CARRIERGATEWAY_H
#define CARRIERGATEWAY_H

#include <sql.h>
#include <sqlext.h>

#include <string>
#include <vector>

#include "../utils/DataBaseConnector.h"
#include "../utils/SqlExecuter.h"

class CarrierGateway {
 public:
  CarrierGateway();  // Конструктор по умолчанию
  bool insertCarrier(const std::string &name);
  bool deleteCarrier(int carrierId);
  bool getCarrier(int carrierId, std::string &name);
  bool updateCarrier(int carrierId, const std::string &name);
  bool getAllCarriers(std::vector<std::tuple<int, std::string>> &carrierData);

 private:
  DataBaseConnector *dbConnector;
  SqlExecuter *sqlExecuter;
};

#endif
