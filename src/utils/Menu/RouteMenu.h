#ifndef RPBD_ROUTEMENU_H
#define RPBD_ROUTEMENU_H

#include "../../gateways/RouteGateway.h"
#include "../../models/Employees.h"
#include "../../models/Hotels.h"
#include "../../models/Route.h"
#include "../IdMapper/EmployeeIdMapper.h"
#include "../IdMapper/FlightIdMapper.h"
#include "../IdMapper/HotelIdMapper.h"
#include "../IdMapper/RouteIdMapper.h"
#include "EmployeeMenu.h"
#include "FlightMenu.h"
#include "HotelMenu.h"

class RouteMenu {
 public:
  explicit RouteMenu(RouteGateway& rGateway) : routeGateway(rGateway) {
    employeeIdMapper = &EmployeeIdMapper::getInstance();
    routeIdMapper = &RouteIdMapper::getInstance();
    hotelIdMapper = &HotelIdMapper::getInstance();
    flightIdMapper = &FlightIdMapper::getInstance();
  }

  void displayMenu() {
    int choice;
    do {
      std::cout << "\nRoute Menu:\n";
      std::cout << "1. Insert Route\n";
      std::cout << "2. Delete Route\n";
      std::cout << "3. Update Route\n";
      std::cout << "4. View All Routes\n";
      std::cout << "5. Exit\n";
      std::cout << "Enter your choice: ";
      std::cin >> choice;

      switch (choice) {
        case 1:
          insertRoute();
          break;
        case 2:
          deleteRoute();
          break;
        case 3:
          updateRoute();
          break;
        case 4:
          viewAllRoutes();
          break;
        case 5:
          std::cout << "Exiting Route Menu.\n";
          break;
        default:
          std::cout << "Invalid choice. Please try again.\n";
      }
    } while (choice != 5);
  }

  void insertRoute() {
    std::string name, country, city;
    int duration, hotelId, flightId, employeeId;

    std::cout << "Insert route name: (required string)\n";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "Insert country: (required string)\n";
    std::getline(std::cin, country);
    std::cout << "Insert city: (required string)\n";
    std::getline(std::cin, city);
    std::cout << "Insert duration: (required int)\n";
    std::cin >> duration;

    HotelMenu::displayAllHotels(*hotelIdMapper);
    std::cout << "Insert hotel ID: (required int)\n";

    std::cin >> hotelId;

    if (!HotelMenu::isHotelIdValid(hotelId, *hotelIdMapper)) {
      std::cerr << "Hotel #" << hotelId << " does not exist.\n";
      if (HotelMenu::displayAllHotels(*hotelIdMapper)) {
        std::cout << "Please choose an existing hotel.\n";
        return;
      } else {
        std::cerr << "No hotels available. Cannot proceed.\n";
        return;
      }
    }

    std::cout << "Insert flight number: (required int)\n";
    FlightMenu::displayAllFlights(*flightIdMapper);
    std::cin >> flightId;

    if (!FlightMenu::isFlightIdValid(flightId, *flightIdMapper)) {
      std::cerr << "Flight #" << flightId << " does not exist.\n";
      if (FlightMenu::displayAllFlights(*flightIdMapper)) {
        std::cout << "Please choose an existing flight ID.\n";
        return;
      } else {
        std::cerr << "No flights available. Cannot proceed.\n";
        return;
      }
    }

    std::cout << "Insert employee number: (required int)\n";
    EmployeeMenu::displayAllEmployees(*employeeIdMapper);
    std::cin >> employeeId;

    if (!EmployeeMenu::isEmployeeIdValid(employeeId, *employeeIdMapper)) {
      std::cerr << "Employee #" << employeeId << " does not exist.\n";
      if (EmployeeMenu::displayAllEmployees(*employeeIdMapper)) {
        std::cout << "Please choose an existing employee.\n";
        return;
      } else {
        std::cerr << "No employees available. Cannot proceed.\n";
        return;
      }
    }

    Route route(routeIdMapper->generateNextAbstractId(), name, country, city,
                duration, hotelId, flightId, employeeId);

    if (routeGateway.insertRoute(name, country, city, duration,
                                 hotelIdMapper->getRealId(hotelId),
                                 flightIdMapper->getRealId(flightId),
                                 employeeIdMapper->getRealId(employeeId))) {
      std::cout << "Route inserted successfully. " << route.getId() << "\n";
      routeIdMapper->routeVector.push_back(route);
      routeIdMapper->addMapping(route.getId(),
                                routeGateway.getLastInsertedId());
    } else {
      std::cerr << "Failed to insert route.\n";
    }
  }

  void deleteRoute() {
    int abstractId;
    displayAllRoutes(*routeIdMapper);
    std::cout << "Insert number to delete\n";
    std::cin >> abstractId;
    if (!isRouteIdValid(abstractId, *routeIdMapper)) {
      std::cerr << "Not existing route #" << abstractId << ".\n";
    }
    if (routeGateway.deleteRoute(routeIdMapper->getRealId(abstractId))) {
      std::cout << "Route #" << abstractId << " deleted successfully.\n";
      for (auto it = routeIdMapper->routeVector.begin();
           it != routeIdMapper->routeVector.end(); ++it) {
        if (it->getId() == abstractId) {
          routeIdMapper->routeVector.erase(it);
          break;
        }
      }
    } else {
      std::cerr << "Failed to delete route with abstract ID " << abstractId
                << ".\n";
    }
  }

  void viewAllRoutes() {
    std::cout << "\nAll Routes:\n";

    for (const Route& route : routeIdMapper->routeVector) {
      int abstractId = route.getId();
      std::string name = route.getName();
      std::string country = route.getCountry();
      std::string city = route.getCity();
      int duration = route.getDuration();
      int hotelId = route.getHotelId();
      int flightId = route.getFlightId();
      int employeeId = route.getEmployeeId();
      std::string empName;
      for (const Employees& employee : employeeIdMapper->employeeVector) {
        if (employee.getId() == employeeId) {
          empName = employee.getFullName();
          break;
        }
      }

      std::string hotelName;
      for (const Hotels& hotel : hotelIdMapper->hotelVector) {
        if (hotel.getId() == hotelId) {
          hotelName = hotel.getName();
          break;
        }
      }

      std::cout << "#" << abstractId << "\n";
      std::cout << "Name: " << name << "\n";
      std::cout << "Country: " << country << "\n";
      std::cout << "City: " << city << "\n";
      std::cout << "Duration: " << duration << " days\n";
      std::cout << "Hotel: " << hotelName << "\n";
      std::cout << "Flight #" << flightId << "\n";
      std::cout << "Employee name: " << empName << "\n";
      std::cout << "\n";
    }
  }

  void updateRoute() {
    displayAllRoutes(*routeIdMapper);
    int routeId;
    std::cout << "Enter the number of the route you want to update: ";
    std::cin >> routeId;

    if (!routeIdMapper->getRealId(routeId)) {
      std::cerr << "Route #" << routeId << " does not exist.\n";
      return;
    }

    int realRouteId = routeIdMapper->getRealId(routeId);

    std::string newName, newCountry, newCity;
    int newDuration, newHotelId, newFlightId, newEmployeeId;

    std::cout << "Enter new name (or press Enter to keep the current value): ";
    std::cin.ignore();
    std::getline(std::cin, newName);

    std::cout
        << "Enter new country (or press Enter to keep the current value): ";
    std::getline(std::cin, newCountry);

    std::cout << "Enter new city (or press Enter to keep the current value): ";
    std::getline(std::cin, newCity);

    std::cout
        << "Enter new duration (or press Enter to keep the current value): ";
    std::string newDurationInput;
    std::getline(std::cin, newDurationInput);

    if (!newDurationInput.empty()) {
      try {
        newDuration = std::stoi(newDurationInput);
      } catch (const std::invalid_argument& e) {
        std::cerr
            << "Invalid input for duration. Please enter a valid integer.\n";
        return;
      }
    }

    std::cout << "Enter new hotel number (or press Enter to keep the current "
                 "value): ";
    std::string newHotelIdInput;
    std::getline(std::cin, newHotelIdInput);
    if (!newHotelIdInput.empty()) {
      try {
        newHotelId = std::stoi(newHotelIdInput);
        std::cout << "00000000000000";
      } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid input for hotel number. Please enter a valid "
                     "integer.\n";
        return;
      }
    }

    std::cout << "Enter new flight number (or press Enter to keep the current "
                 "value): ";
    std::string newFlightIdInput;
    std::getline(std::cin, newFlightIdInput);

    if (!newFlightIdInput.empty()) {
      try {
        newFlightId = std::stoi(newFlightIdInput);
      } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid input for flight number. Please enter a valid "
                     "integer.\n";
        return;
      }
    }

    std::cout << "Enter new employee number (or press Enter to keep the "
                 "current value): ";
    std::string newEmployeeIdInput;
    std::getline(std::cin, newEmployeeIdInput);

    if (!newEmployeeIdInput.empty()) {
      try {
        newEmployeeId = std::stoi(newEmployeeIdInput);
      } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid input for employee number. Please enter a valid "
                     "integer.\n";
        return;
      }
    }

    for (Route& route : routeIdMapper->routeVector) {
      if (route.getId() == routeIdMapper->getAbstractId(realRouteId)) {
        if (!newName.empty()) {
          route.setName(newName);
        } else {
          newName = route.getName();
        }

        if (!newCountry.empty()) {
          route.setCountry(newCountry);
        } else {
          newCountry = route.getCountry();
        }

        if (!newCity.empty()) {
          route.setCity(newCity);
        } else {
          newCity = route.getCity();
        }

        if (newDuration != 0) {
          route.setDuration(newDuration);
        } else {
          newDuration = route.getDuration();
        }
        if (!newHotelIdInput.empty()) {
          route.setHotelId(newHotelId);
        } else {
          newHotelId = route.getHotelId();
        }

        if (!newFlightIdInput.empty()) {
          route.setFlightId(newFlightId);
        } else {
          newFlightId = route.getFlightId();
        }

        if (!newEmployeeIdInput.empty()) {
          route.setEmployeeId(newEmployeeId);
        } else {
          newEmployeeId = route.getEmployeeId();
        }

        break;
      }
    }

    if (routeGateway.updateRoute(realRouteId, newName, newCountry, newCity,
                                 newDuration,
                                 hotelIdMapper->getRealId(newHotelId),
                                 flightIdMapper->getRealId(newFlightId),
                                 employeeIdMapper->getRealId(newEmployeeId))) {
      std::cout << "Route #" << routeId << " updated successfully.\n";
    } else {
      std::cerr << "Failed to update route #" << routeId << ".\n";
    }
  }

  static bool isRouteIdValid(int routeId, const RouteIdMapper& idMapper) {
    for (const Route& route : idMapper.routeVector) {
      if (route.getId() == routeId) {
        return true;
      }
    }
    return false;
  }

  static bool displayAllRoutes(const RouteIdMapper& idMapper) {
    if (idMapper.routeVector.empty()) {
      std::cerr << "No routes available. Cannot proceed.\n";
      return false;
    }

    std::cout << "Available Routes:\n";
    for (const Route& route : idMapper.routeVector) {
      std::cout << "{#" << route.getId() << "; name=\"" << route.getName()
                << "\"}\n";
    }
    return true;
  }

 private:
  EmployeeIdMapper* employeeIdMapper;
  RouteIdMapper* routeIdMapper;
  HotelIdMapper* hotelIdMapper;
  FlightIdMapper* flightIdMapper;
  RouteGateway& routeGateway;
};

#endif  // RPBD_ROUTEMENU_H