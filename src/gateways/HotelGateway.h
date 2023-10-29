#ifndef HOTELGATEWAY_H
#define HOTELGATEWAY_H

#include <sql.h>
#include <sqlext.h>

#include <string>
#include <vector>

#include "../utils/DataBaseConnector.h"
#include "../utils/SqlExecuter.h"

class HotelGateway {
 public:
  HotelGateway();

  bool insertHotel(const std::string &name, int hotelClass,
                   const std::string &roomCategory);
  bool deleteHotel(int hotelId);
  bool getHotel(int hotelId, std::string &name, int &hotelClass,
                std::string &roomCategory);
  bool updateHotel(int hotelId, const std::string &name, int hotelClass,
                   const std::string &roomCategory);
  bool getAllHotels(
      std::vector<std::tuple<int, std::string, int, std::string>> &hotelData);
  bool findHotelsByName(const std::string &name,
                        std::vector<int> &matchingHotelIds);

 private:
  DataBaseConnector *dbConnector;
  SqlExecuter *sqlExecuter;
};

#endif
