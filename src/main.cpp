#include <iostream>

#include "./gateways/AircraftGateway.h"
#include "./gateways/CarrierGateway.h"
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

    HotelGateway hotelGateway;

    // Тест метода insertHotel
    if (hotelGateway.insertHotel("Hotel A", 4, "Standard")) {
      std::cout << "Hotel inserted successfully." << std::endl;
    } else {
      std::cerr << "Failed to insert hotel." << std::endl;
    }

    // Тест метода deleteHotel
    if (hotelGateway.deleteHotel(1)) {
      std::cout << "Hotel deleted successfully." << std::endl;
    } else {
      std::cerr << "Failed to delete hotel." << std::endl;
    }

    // Тест метода getHotel
    int hotelId = 5;
    std::string name;
    int hotelClass;
    std::string roomCategory;

    if (hotelGateway.getHotel(hotelId, name, hotelClass, roomCategory)) {
      std::cout << "Hotel get successfully." << std::endl;
      std::cout << "Hotel ID: " << hotelId << std::endl;
      std::cout << "Name: " << name << std::endl;
      std::cout << "Hotel Class: " << hotelClass << std::endl;
      std::cout << "Room Category: " << roomCategory << std::endl;
    } else {
      std::cerr << "Failed to get hotel." << std::endl;
    }

    // Тест метода updateHotel
    if (hotelGateway.updateHotel(5, "Updated Hotel", 5, "Luxury")) {
      std::cout << "Hotel updated successfully." << std::endl;
    } else {
      std::cerr << "Failed to update hotel." << std::endl;
    }

    // Тест метода getAllHotels
    std::vector<int> hotelIds;
    if (hotelGateway.getAllHotels(hotelIds)) {
      std::cout << "Hotels were found successfully." << std::endl;
      for (int id : hotelIds) {
        std::cout << "Hotel ID: " << id << std::endl;
      }
    } else {
      std::cerr << "Failed to get all hotels." << std::endl;
    }

    // Тест метода findHotelsByName
    std::vector<int> matchingHotelIds;
    if (hotelGateway.findHotelsByName("Updated Hotel", matchingHotelIds)) {
      std::cout << "Hotels by name were found successfully." << std::endl;
      for (int id : matchingHotelIds) {
        std::cout << "Matching Hotel ID: " << id << std::endl;
      }
    } else {
      std::cerr << "Failed to find hotels by name." << std::endl;
    }

    dbConnector.disconnect();
  } else {
    std::cerr << "Failed to connect to the database." << std::endl;
    return 1;
  }

  return 0;
}