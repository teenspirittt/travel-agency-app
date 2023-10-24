#ifndef AIRCRAFTGATEWAY_H
#define AIRCRAFTGATEWAY_H

#include <sql.h>
#include <sqlext.h>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

class AircraftGateway {
 public:
  AircraftGateway(SQLHDBC hdbc);
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
  SQLHDBC hdbc;
};

#endif
