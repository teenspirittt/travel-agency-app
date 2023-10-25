#include <iostream>

#include "./gateways/AircraftGateway.h"
#include "./gateways/CarrierGateway.h"
#include "./gateways/ClientGateway.h"
#include "./gateways/EmployeeGateway.h"
#include "./gateways/EmployeeTransfers.h"
#include "./gateways/FlightsGateway.h"
#include "./gateways/HotelGateway.h"
#include "./models/Aircraft.h"
#include "./utils/DataBaseConnector.h"
#include "./utils/DataBaseInitializer.h"

int main() {
  DataBaseConnector& dbConnector = DataBaseConnector::getInstance();

  if (!dbConnector.readConfigFile("./database.properties")) {
    std::cerr << "Failed to read configuration file." << std::endl;
    return 1;
  }

  if (dbConnector.connect()) {
    std::cout << "Connected to the database." << std::endl;
    DataBaseInitializer dbInitializer(dbConnector);

    if (dbInitializer.initializeDataBase()) {
      std::cout << "Init database";
    } else {
      std::cout << "Init database error";
    }

    dbConnector.disconnect();
  } else {
    std::cerr << "Failed to connect to the database." << std::endl;
    return 0;
  }
}