#include <iostream>

#include "./gateways/AircraftGateway.h"
#include "./utils/DataBaseConnector.h"
#include "./utils/DataBaseInitializer.h"

int main() {
  DataBaseConnector dbConnector;

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

    AircraftGateway aircraftGateway(dbConnector.getHDBC());

    if (aircraftGateway.insertAircraft("A380", 12, "Airbus", 555)) {
      std::cout << "Aircraft inserted successfully." << std::endl;
    } else {
      std::cerr << "Failed to insert aircraft." << std::endl;
    }

    dbConnector.disconnect();
  } else {
    std::cerr << "Failed to connect to the database." << std::endl;
    return 1;
  }

  return 0;
}