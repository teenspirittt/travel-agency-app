#ifndef RPBD_HOTELMENU_H
#define RPBD_HOTELMENU_H

#include "../gateways/HotelGateway.h"
#include "../models/Hotels.h"
#include "HotelIdMapper.h"

class HotelMenu {
 public:
  explicit HotelMenu(HotelGateway &hGateway) : hotelGateway(hGateway) {
    hotelIdMapper = &HotelIdMapper::getInstance();
  }

  void displayMenu() {
    int choice;
    do {
      std::cout << "\nHotel Menu:\n";
      std::cout << "1. Insert Hotel\n";
      std::cout << "2. Delete Hotel\n";
      std::cout << "3. Update Hotel\n";
      std::cout << "4. View All Hotels\n";
      std::cout << "5. Exit\n";
      std::cout << "Enter your choice: ";
      std::cin >> choice;

      switch (choice) {
        case 1:
          insertHotel();
          break;
        case 2:
          deleteHotel();
          break;
        case 3:
          updateHotel();
          break;
        case 4:
          viewAllHotels();
          break;
        case 5:
          std::cout << "Exiting Hotel Menu.\n";
          break;
        default:
          std::cout << "Invalid choice. Please try again.\n";
      }
    } while (choice != 5);
  }

  void insertHotel() {
    std::string name;
    std::cout << "Insert hotel name: (required string)\n";
    std::cin.ignore();
    std::getline(std::cin, name);

    int hotelClass;
    std::cout << "Insert hotel class: (required int)\n";
    std::cin >> hotelClass;

    std::string roomCategory;
    std::cout << "Insert hotel room catregory: (required string)\n";
    std::cin.ignore();
    std::getline(std::cin, roomCategory);

    if (hotelGateway.insertHotel(name, hotelClass, roomCategory)) {
      std::cout << "Carrier inserted successfully.\n";

      Hotels newHotel(hotelIdMapper->generateNextAbstractId(), name, hotelClass,
                      roomCategory);
      hotelIdMapper->hotelVector.push_back(newHotel);
      hotelIdMapper->addMapping(newHotel.getId(),
                                hotelGateway.getLastInsertedId());
    }
  }

  void deleteHotel() {
    int abstractId;
    std::cout << "Insert ID to delete\n";
    std::cin >> abstractId;

    if (hotelGateway.deleteHotel(hotelIdMapper->getRealId(abstractId))) {
      std::cout << "Hotel with abstract ID " << abstractId
                << " deleted successfully.\n";

      for (auto it = hotelIdMapper->hotelVector.begin();
           it != hotelIdMapper->hotelVector.end(); ++it) {
        if (it->getId() == abstractId) {
          hotelIdMapper->hotelVector.erase(it);
          break;
        }
      }
    } else {
      std::cerr << "Failed to delete hotel with abstract ID " << abstractId
                << ".\n";
    }
  }

  void viewAllHotels() {
    std::cout << "\nAll Hotels:\n";

    for (const Hotels &hotel : hotelIdMapper->hotelVector) {
      int abstractId = hotel.getId();
      std::string name = hotel.getName();
      int hotelClass = hotel.getHotelClass();
      std::string roomCategory = hotel.getRoomCategory();

      std::cout << "Abstract ID: " << abstractId << "\n";
      std::cout << "Name: " << name << "\n";
      std::cout << "Hotel class: " << hotelClass << "\n";
      std::cout << "Room category: " << roomCategory << "\n";
      std::cout << "\n";
    }
  }

  void updateHotel() {
    int abstractId;
    std::cout << "Enter the ID of the hotel you want to update: ";
    std::cin >> abstractId;

    if (!hotelIdMapper->getRealId(abstractId)) {
      std::cerr << "Hotel with abstract ID " << abstractId
                << " does not exist.\n";
      return;
    }

    int realHotelId = hotelIdMapper->getRealId(abstractId);
    std::string newName, newRoomCategory;
    int newHotelClass;

    std::cout << "Enter new name (or press Enter to keep the current value): ";
    std::cin.ignore();
    std::getline(std::cin, newName);

    std::cout
        << "Enter new hotel class (or press Enter to keep the current value): ";
    std::string hotelClassInput;
    std::getline(std::cin, hotelClassInput);

    if (!hotelClassInput.empty()) {
      try {
        newHotelClass = std::stoi(hotelClassInput);
      } catch (const std::invalid_argument &e) {
        std::cerr
            << "Invalid input for hotel class. Please enter a valid integer.\n";
        return;
      }
    }

    std::cout << "Enter new room category (or press Enter to keep the current "
                 "value): ";
    std::getline(std::cin, newRoomCategory);

    for (Hotels &hotel : hotelIdMapper->hotelVector) {
      if (hotel.getId() == hotelIdMapper->getAbstractId(realHotelId)) {
        if (!newName.empty()) {
          hotel.setName(newName);
        } else {
          newName = hotel.getName();
        }
        if (newHotelClass != 0) {
          hotel.setHotelClass(newHotelClass);
        } else {
          newHotelClass = hotel.getHotelClass();
        }
        if (!newRoomCategory.empty()) {
          hotel.setRoomCategory(newRoomCategory);
        } else {
          newRoomCategory = hotel.getRoomCategory();
        }
        break;
      }
    }

    if (hotelGateway.updateHotel(realHotelId, newName, newHotelClass,
                                 newRoomCategory)) {
      std::cout << "Hotel with abstract ID " << abstractId
                << " updated successfully.\n";
    } else {
      std::cerr << "Failed to update hotel with abstract ID " << abstractId
                << ".\n";
    }
  }

  static bool isHotelIdValid(int hotelId, const HotelIdMapper &idMapper) {
    for (const Hotels &hotel : idMapper.hotelVector) {
      if (hotel.getId() == hotelId) {
        return true;
      }
    }
    return false;
  }

  static bool displayAllHotels(const HotelIdMapper &idMapper) {
    if (idMapper.hotelVector.empty()) {
      std::cerr << "No hotels available. Cannot proceed.\n";
      return false;
    }

    std::cout << "Available Hotels:\n";
    for (const Hotels &hotel : idMapper.hotelVector) {
      std::cout << "{id=" << hotel.getId() << "; name=\"" << hotel.getName()
                << "\"}\n";
    }
    return true;
  }

 private:
  HotelGateway &hotelGateway;
  HotelIdMapper *hotelIdMapper;
};

#endif  // RPBD_HOTELMENU_H