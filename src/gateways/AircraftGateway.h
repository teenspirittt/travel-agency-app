#ifndef AIRCRAFTGATEWAY_H
#define AIRCRAFTGATEWAY_H

#include <sql.h>
#include <sqlext.h>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "../utils/DataBaseConnector.h"
#include "../utils/SqlExecuter.h"

class AircraftGateway {
 public:
  AircraftGateway();
  bool insertAircraft(const std::string &aircraftType, int carrierId,
                      const std::string &manufacturer, int capacity);
  bool deleteAircraft(int aircraftId);

  bool getAircraft(int aircraftId, std::string &aircraftType, int &carrierId,
                   std::string &manufacturer, int &capacity);

  bool updateAircraft(int aircraftId, const std::string &aircraftType,
                      int carrierId, const std::string &manufacturer,
                      int capacity);
  bool getAllAircraft(std::vector<int> &aircraftIds);

  bool findAircraftByType(const std::string &aircraftType,
                          std::vector<int> &matchingAircraftIds);
  bool findAircraftByCarrier(int carrierId,
                             std::vector<int> &matchingAircraftIds);

 private:
  DataBaseConnector *dbConnector;
  SqlExecuter *sqlExecuter;
};

#endif
