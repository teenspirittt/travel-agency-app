#pragma once

#include "../gateways/AircraftGateway.h"
#include "../gateways/CarrierGateway.h"
#include "../models/Aircraft.h"
#include "AircraftIdMapper.h"
#include "CarrierIdMapper.h"

class AircraftMenu {
 public:
  AircraftMenu(AircraftGateway &aGateway, CarrierGateway &cGateway)
      : aircraftGateway(aGateway), carrierGateway(cGateway) {
    aircraftIdMapper = &AircraftIdMapper::getInstance();
    carrierIdMapper = &CarrierIdMapper::getInstance();
  }

  void displayMenu() {
    int choice;
    do {
      std::cout << "Aircraft Menu:\n";
      std::cout << "1. Insert Aircraft\n";
      std::cout << "2. Delete Aircraft\n";
      std::cout << "3. Update Aircraft\n";
      std::cout << "4. View All Aircraft\n";
      std::cout << "5. Exit\n";
      std::cout << "Enter your choice: ";
      std::cin >> choice;

      switch (choice) {
        case 1:
          insertAircraft();
          break;
        case 2:
          deleteAircraft();
          break;
        case 3:
          updateAircraft();
          break;
        case 4:
          viewAllAircraft();
          break;
        case 5:
          std::cout << "Exiting Aircraft Menu.\n";
          break;
        default:
          std::cout << "Invalid choice. Please try again.\n";
      }
    } while (choice != 4);
  }

  void insertAircraft() {
    std::string aircraftType, manufacturer;
    int carrierId, capacity;

    std::cout << "Insert aircraft type: (required string)\n";
    std::cin.ignore();
    std::getline(std::cin, aircraftType);
    std::cout << "Insert manufacturer: (required string)\n";
    std::getline(std::cin, manufacturer);

    if (!isCarrierIdValid(carrierId)) {
      std::cerr << "Carrier with ID " << carrierId << " does not exist.\n";
      if (displayAllCarriers()) {
        std::cout << "Please choose an existing carrier ID.\n";
        return;
      } else {
        std::cerr << "No carriers available. Cannot proceed.\n";
        return;
      }
    }

    std::cout << "Insert capacity: (required int)\n";
    std::cin >> capacity;

    Aircraft aircraft(aircraftIdMapper->generateNextAbstractId(), aircraftType,
                      carrierIdMapper->getAbstractId(carrierId), manufacturer,
                      capacity);

    if (aircraftGateway.insertAircraft(
            aircraft.getAircraftType(), aircraft.getCarrierId(),
            aircraft.getManufacturer(), aircraft.getCapacity())) {
      std::cout << "Aircraft inserted successfully. " << aircraft.getId()
                << "\n";
    } else {
      std::cerr << "Failed to insert aircraft.\n";
    }
  }

  void deleteAircraft() {
    // Здесь можно запросить абстрактный ID Aircraft, который нужно удалить
    int abstractId;
    // Запрос абстрактного ID у пользователя...

    // Вызываем метод для удаления
    if (aircraftGateway.deleteAircraft(
            aircraftIdMapper->getRealId(abstractId))) {
      std::cout << "Aircraft with abstract ID " << abstractId
                << " deleted successfully.\n";
    } else {
      std::cerr << "Failed to delete aircraft with abstract ID " << abstractId
                << ".\n";
    }
  }

  void viewAllAircraft() {
    // Получаем все записи Aircraft
    std::vector<std::tuple<int, std::string, int, std::string, int>>
        aircraftData;
    if (aircraftGateway.getAllAircraft(aircraftData)) {
      std::cout << "All Aircraft:\n";
      for (const auto &aircraft : aircraftData) {
        // Распаковываем кортеж с данными
        int abstractId = aircraftIdMapper->getAbstractId(std::get<0>(aircraft));
        std::string aircraftType = std::get<1>(aircraft);
        int abstractCarrierId = std::get<2>(aircraft);
        std::string manufacturer = std::get<3>(aircraft);
        int capacity = std::get<4>(aircraft);

        // Выводим данные
        std::cout << "Abstract ID: " << abstractId << "\n";
        std::cout << "Aircraft Type: " << aircraftType << "\n";
        std::cout << "Carrier ID: " << abstractCarrierId << "\n";
        std::cout << "Manufacturer: " << manufacturer << "\n";
        std::cout << "Capacity: " << capacity << "\n";
        std::cout << "\n";
      }
    } else {
      std::cerr << "Failed to get aircraft data.\n";
    }
  }

  bool isCarrierIdValid(int carrierId) {
    std::vector<std::tuple<int, std::string>> carrierData;
    if (carrierGateway.getAllCarriers(carrierData)) {
      for (const auto &carrier : carrierData) {
        if (std::get<0>(carrier) == carrierId) {
          return true;
        }
      }
    }
    return false;
  }

  bool displayAllCarriers() {
    std::vector<std::tuple<int, std::string>> carrierData;
    if (carrierGateway.getAllCarriers(carrierData)) {
      std::cout << "Available Carriers:\n";
      for (const auto &carrier : carrierData) {
        std::cout << "{id="
                  << carrierIdMapper->getAbstractId(std::get<0>(carrier))
                  << "; name=\"" << std::get<1>(carrier) << "\"}\n";
      }
      return true;
    } else {
      std::cerr << "Failed to get carrier data.\n";
      return false;
    }
  }

  void updateAircraft() {}

 private:
  AircraftGateway &aircraftGateway;
  CarrierGateway &carrierGateway;
  AircraftIdMapper *aircraftIdMapper;
  CarrierIdMapper *carrierIdMapper;
};
