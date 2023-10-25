#include <iostream>

#include "./gateways/AircraftGateway.h"
#include "./gateways/CarrierGateway.h"
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

    AircraftGateway aircraftGateway;

    if (aircraftGateway.insertAircraft("A380", 12, "Airbus", 555)) {
      std::cout << "Aircraft inserted successfully." << std::endl;
    } else {
      std::cerr << "Failed to insert aircraft." << std::endl;
    }

    if (aircraftGateway.deleteAircraft(12)) {
      std::cout << "Aircraft deleted successfully." << std::endl;
    } else {
      std::cerr << "Failed to delete aircraft." << std::endl;
    }

    Aircraft aircraft(4);
    std::string aircraftType;
    int carrierId;
    std::string manufacturer;
    int capacity;

    if (aircraftGateway.getAircraft(4, aircraftType, carrierId, manufacturer,
                                    capacity)) {
      std::cout << "Aircraft get successfully." << std::endl;
      std::cout << "Aircraft ID: " << aircraft.getId() << std::endl;
      std::cout << "Aircraft Type: " << aircraftType << std::endl;
      std::cout << "Carrier ID: " << carrierId << std::endl;
      std::cout << "Manufacturer: " << manufacturer << std::endl;
      std::cout << "Capacity: " << capacity << std::endl;
    } else {
      std::cerr << "Failed to get aircraft." << std::endl;
    }

    if (aircraftGateway.updateAircraft(10, "Boeing 747", 12, "Boeing", 500)) {
      std::cout << "Aircraft updated successfully." << std::endl;
    } else {
      std::cerr << "Failed to update aircraft." << std::endl;
    }

    std::vector<int> matchingAircraftIds;
    if (aircraftGateway.findAircraftByType("Boeing 747", matchingAircraftIds)) {
      std::cout << "Aircrafts by type was found successfully." << std::endl;
      for (int aircraftId : matchingAircraftIds) {
        std::cout << "Matching Aircraft ID: " << aircraftId << std::endl;
      }
    } else {
      std::cerr << "Failed to find aircraft." << std::endl;
    }

    std::vector<int> aircraftIds;
    if (aircraftGateway.getAllAircraft(aircraftIds)) {
      std::cout << "Aircrafts was found successfully." << std::endl;

      for (int aircraftId : aircraftIds) {
        std::cout << "Aircraft ID: " << aircraftId << std::endl;
      }

    } else {
      std::cerr << "Failed to get all aircraft.s" << std::endl;
    }

    std::vector<int> matchingAircraftIds2;
    if (aircraftGateway.findAircraftByCarrier(12, matchingAircraftIds2)) {
      std::cout << "Aircrafts by carrier was found successfully." << std::endl;
      for (int aircraftId : matchingAircraftIds2) {
        std::cout << "Matching Aircraft ID: " << aircraftId << std::endl;
      }
    } else {
      std::cerr << "Failed to find aircraft." << std::endl;
    }

    CarrierGateway carrierGateway;

    if (carrierGateway.insertCarrier("Emirates")) {
      std::cout << "Carrier inserted successfully." << std::endl;
    } else {
      std::cerr << "Failed to insert carrier." << std::endl;
    }

    if (carrierGateway.deleteCarrier(1)) {
      std::cout << "Carrier deleted successfully." << std::endl;
    } else {
      std::cerr << "Failed to delete carrier." << std::endl;
    }

    std::string carrierName;
    if (carrierGateway.getCarrier(12, carrierName)) {
      std::cout << "Carrier get successfully." << std::endl;
      std::cout << "Carrier Name: " << carrierName << std::endl;
    } else {
      std::cerr << "Failed to get carrier." << std::endl;
    }

    if (carrierGateway.updateCarrier(12, "Qatar Airways")) {
      std::cout << "Carrier updated successfully." << std::endl;
    } else {
      std::cerr << "Failed to update carrier." << std::endl;
    }

    std::vector<int> carrierIds;
    if (carrierGateway.getAllCarriers(carrierIds)) {
      std::cout << "Carriers were found successfully." << std::endl;
      for (int carrierId : carrierIds) {
        std::cout << "Carrier ID: " << carrierId << std::endl;
      }
    } else {
      std::cerr << "Failed to get all carriers." << std::endl;
    }

    dbConnector.disconnect();
  } else {
    std::cerr << "Failed to connect to the database." << std::endl;
    return 1;
  }

  return 0;
}