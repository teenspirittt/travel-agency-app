#ifndef RPBD_CARRIERMENU_H
#define RPBD_CARRIERMENU_H

#include "../../gateways/CarrierGateway.h"
#include "../IdMapper/CarrierIdMapper.h"

class CarrierMenu {
 public:
  explicit CarrierMenu(CarrierGateway &cGateway) : carrierGateway(cGateway) {
    carrierIdMapper = &CarrierIdMapper::getInstance();
  }

  void displayMenu() {
    int choice;
    do {
      std::cout << "\nCarrier Menu:\n";
      std::cout << "1. Insert Carrier\n";
      std::cout << "2. Delete Carrier\n";
      std::cout << "3. Update Carrier\n";
      std::cout << "4. View All Carriers\n";
      std::cout << "5. Exit\n";
      std::cout << "Enter your choice: \n";
      std::cin >> choice;

      switch (choice) {
        case 1:
          insertCarrier();
          break;
        case 2:
          deleteCarrier();
          break;
        case 3:
          updateCarrier();
          break;
        case 4:
          viewAllCarriers();
          break;
        case 5:
          std::cout << "Exiting Carrier Menu.\n";
          break;
        default:
          std::cout << "Invalid choice. Please try again.\n";
      }
    } while (choice != 5);
  }

  void insertCarrier() {
    std::string name;
    std::cout << "Insert carrier name: (required string)\n";
    std::cin.ignore();
    std::getline(std::cin, name);

    if (carrierGateway.insertCarrier(name)) {
      std::cout << "Carrier inserted successfully.\n";

      Carrier newCarrier(carrierIdMapper->generateNextAbstractId(), name);
      carrierIdMapper->carrierVector.push_back(newCarrier);
      carrierIdMapper->addMapping(newCarrier.getId(),
                                  carrierGateway.getLastInsertedId());
    }
  }

  void deleteCarrier() {
    int abstractId;
    displayAllCarriers(*carrierIdMapper);
    std::cout << "Insert ID to delete\n";
    std::cin >> abstractId;

    if (!isCarrierIdValid(abstractId, *carrierIdMapper)) {
      std::cerr << "Not existing carrier #" << abstractId << ".\n";
    }

    if (carrierGateway.deleteCarrier(carrierIdMapper->getRealId(abstractId))) {
      std::cout << "Carrier #" << abstractId << " deleted successfully.\n";

      for (auto it = carrierIdMapper->carrierVector.begin();
           it != carrierIdMapper->carrierVector.end(); ++it) {
        if (it->getId() == abstractId) {
          carrierIdMapper->carrierVector.erase(it);
          break;
        }
      }
    } else {
      std::cerr << "Failed to delete carrier." << abstractId << ".\n";
    }
  }

  void viewAllCarriers() {
    std::cout << "\nAll Carriers:\n";

    for (const Carrier &carrier : carrierIdMapper->carrierVector) {
      int abstractId = carrier.getId();
      std::string name = carrier.getName();

      std::cout << "#" << abstractId << "\n";
      std::cout << "Name: " << name << "\n";
      std::cout << "\n";
    }
  }

  void updateCarrier() {
    int abstractId;
    std::cout << "Enter the number of the carrier you want to update: ";
    std::cin >> abstractId;

    if (!carrierIdMapper->getRealId(abstractId)) {
      std::cerr << "Carrier #" << abstractId << " does not exist.\n";
      return;
    }

    int realCarrierId = carrierIdMapper->getRealId(abstractId);
    std::string newName;

    std::cout << "Enter new name (or press Enter to keep the current value): ";
    std::cin.ignore();
    std::getline(std::cin, newName);

    for (Carrier &carrier : carrierIdMapper->carrierVector) {
      if (carrier.getId() == carrierIdMapper->getAbstractId(realCarrierId)) {
        if (!newName.empty()) {
          carrier.setName(newName);
        } else {
          newName = carrier.getName();
        }
        break;
      }
    }

    if (carrierGateway.updateCarrier(realCarrierId, newName)) {
      std::cout << "Carrier #" << abstractId << " updated successfully.\n";
    } else {
      std::cerr << "Failed to update carrier #" << abstractId << ".\n";
    }
  }

  static bool isCarrierIdValid(int carrierId, const CarrierIdMapper &idMapper) {
    for (const Carrier &carrier : idMapper.carrierVector) {
      if (carrier.getId() == carrierId) {
        return true;
      }
    }
    return false;
  }

  static bool displayAllCarriers(const CarrierIdMapper &idMapper) {
    if (idMapper.carrierVector.empty()) {
      std::cerr << "No carriers available. Cannot proceed.\n";
      return false;
    }

    std::cout << "Available Carriers:\n";
    for (const Carrier &carrier : idMapper.carrierVector) {
      std::cout << "{#" << carrier.getId() << "; name=\"" << carrier.getName()
                << "\"}\n";
    }
    return true;
  }

 private:
  CarrierGateway &carrierGateway;
  CarrierIdMapper *carrierIdMapper;
};

#endif  // RPBD_CARRIERMENU_H
