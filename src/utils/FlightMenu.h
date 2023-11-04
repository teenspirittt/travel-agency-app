#ifndef RPBD_FLIGHTMENU_H
#define RPBD_FLIGHTMENU_H

#include "../gateways/AircraftGateway.h"
#include "../gateways/CarrierGateway.h"
#include "../gateways/FlightsGateway.h"
#include "../models/Flights.h"
#include "AircraftMenu.h"
#include "CarrierMenu.h"
#include "FlightIdMapper.h"

class FlightMenu {
 public:
  explicit FlightMenu(FlightsGateway &fGateway) : flightGateway(fGateway) {
    flightIdMapper = &FlightIdMapper::getInstance();
    carrierIdMapper = &CarrierIdMapper::getInstance();
    aircraftIdMapper = &AircraftIdMapper::getInstance();
  }

  void displayMenu() {
    int choice;
    do {
      std::cout << "\nFlight Menu:\n";
      std::cout << "1. Insert Flight\n";
      std::cout << "2. Delete Flight\n";
      std::cout << "3. Update Flight\n";
      std::cout << "4. View All Flights\n";
      std::cout << "5. Exit\n";
      std::cout << "Enter your choice: ";
      std::cin >> choice;

      switch (choice) {
        case 1:
          insertFlight();
          break;
        case 2:
          deleteFlight();
          break;
        case 3:
          updateFlight();
          break;
        case 4:
          viewAllFlights();
          break;
        case 5:
          std::cout << "Exiting Flight Menu.\n";
          break;
        default:
          std::cout << "Invalid choice. Please try again.\n";
      }
    } while (choice != 5);
  }

  void insertFlight() {
    std::string flightNumber, departureDate, flightClass;
    int aircraftId, availableSeats;

    std::cout << "Insert flight number: (required string)\n";
    std::cin.ignore();
    std::getline(std::cin, flightNumber);
    std::cout << "Insert departure date: (required string)\n";
    std::getline(std::cin, departureDate);

    std::cout << "Insert aircraft ID: (required int)\n";
    AircraftMenu::displayAllAircraft(*aircraftIdMapper);
    std::cin >> aircraftId;

    if (!AircraftMenu::isAircraftIdValid(aircraftId, *aircraftIdMapper)) {
      std::cerr << "Aircraft with ID " << aircraftId << " does not exist.\n";
      if (AircraftMenu::displayAllAircraft(*aircraftIdMapper)) {
        std::cout << "Please choose an existing aircraft ID.\n";
        return;
      } else {
        std::cerr << "No aircraft available. Cannot proceed.\n";
        return;
      }
    }

    std::cout << "Insert flight class: (required string)\n";
    std::cin.ignore();
    std::getline(std::cin, flightClass);

    std::cout << "Insert available seats: (required int)\n";
    std::cin >> availableSeats;

    Flights flight(flightIdMapper->generateNextAbstractId(), flightNumber,
                   departureDate, aircraftIdMapper->getRealId(aircraftId),
                   flightClass, availableSeats);

    if (flightGateway.insertFlight(
            flight.getFlightNumber(), flight.getDepartureDate(),
            flight.getAircraftId(), flight.getFlightClass(),
            flight.getAvailableSeats())) {
      std::cout << "Flight inserted successfully. " << flight.getId() << "\n";
      flightIdMapper->flightVector.push_back(flight);
      flightIdMapper->addMapping(flight.getId(),
                                 flightGateway.getLastInsertedId());
    } else {
      std::cerr << "Failed to insert flight.\n";
    }
  }

  void deleteFlight() {
    int abstractId;
    std::cout << "Insert id to delete\n";
    std::cin >> abstractId;

    for (auto it = flightIdMapper->flightVector.begin();
         it != flightIdMapper->flightVector.end(); ++it) {
      if (it->getId() == abstractId) {
        flightIdMapper->flightVector.erase(it);
        break;
      }
    }

    if (flightGateway.deleteFlight(flightIdMapper->getRealId(abstractId))) {
      std::cout << "Flight with abstract ID " << abstractId
                << " deleted successfully.\n";
    } else {
      std::cerr << "Failed to delete flight with abstract ID " << abstractId
                << ".\n";
    }
  }

  void updateFlight() {
    int flightId;
    std::cout << "Enter the ID of the flight you want to update: ";
    std::cin >> flightId;

    if (!flightIdMapper->getRealId(flightId)) {
      std::cerr << "Flight with ID " << flightId << " does not exist.\n";
      return;
    }

    int realFlightId = flightIdMapper->getRealId(flightId);
    std::cout << realFlightId;

    std::string newFlightNumber, newDepartureDate, newFlightClass;
    int newAircraftId, newAvailableSeats, newCarrierId;

    std::cout << "Enter new flight number (or press Enter to keep the current "
                 "value): ";
    std::cin.ignore();
    std::getline(std::cin, newFlightNumber);

    std::cout << "Enter new departure date (or press Enter to keep the current "
                 "value): ";
    std::getline(std::cin, newDepartureDate);

    std::cout << "Enter new flight class (or press Enter to keep the current "
                 "value): ";
    std::getline(std::cin, newFlightClass);

    std::cout
        << "Enter new aircraft ID (or press Enter to keep the current value): ";
    std::string newAircraftIdInput;
    std::getline(std::cin, newAircraftIdInput);

    if (!newAircraftIdInput.empty()) {
      try {
        newAircraftId = std::stoi(newAircraftIdInput);
      } catch (const std::invalid_argument &e) {
        std::cerr
            << "Invalid input for aircraft ID. Please enter a valid integer.\n";
        return;
      }
    }

    std::cout << "Enter new available seats (or press Enter to keep the "
                 "current value): ";
    std::string newAvailableSeatsInput;
    std::getline(std::cin, newAvailableSeatsInput);

    if (!newAvailableSeatsInput.empty()) {
      try {
        newAvailableSeats = std::stoi(newAvailableSeatsInput);
      } catch (const std::invalid_argument &e) {
        std::cerr << "Invalid input for available seats. Please enter a valid "
                     "integer.\n";
        return;
      }
    }

    for (Flights &flight : flightIdMapper->flightVector) {
      if (flight.getId() == flightIdMapper->getAbstractId(realFlightId)) {
        if (!newFlightNumber.empty()) {
          flight.setFlightNumber(newFlightNumber);
        } else {
          newFlightNumber = flight.getFlightNumber();
        }

        if (!newDepartureDate.empty()) {
          flight.setDepartureDate(newDepartureDate);
        } else {
          newDepartureDate = flight.getDepartureDate();
        }

        if (!newFlightClass.empty()) {
          flight.setFlightClass(newFlightClass);
        } else {
          newFlightClass = flight.getFlightClass();
        }

        if (newAircraftId != 0) {
          flight.setAircraftId(newAircraftId);
        } else {
          newAircraftId = flight.getAircraftId();
        }

        if (newAvailableSeats != 0) {
          flight.setAvailableSeats(newAvailableSeats);
        } else {
          newAvailableSeats = flight.getAvailableSeats();
        }
        break;
      }
    }

    if (flightGateway.updateFlight(
            realFlightId, newFlightNumber, newDepartureDate, newAircraftId,
            newFlightClass, newAvailableSeats, newCarrierId)) {
      std::cout << "Flight with ID " << flightId << " updated successfully.\n";
    } else {
      std::cerr << "Failed to update flight with ID " << flightId << ".\n";
    }
  }

  void viewAllFlights() {
    std::cout << "\nAll Flights:\n";

    for (Flights &flight : flightIdMapper->flightVector) {
      int abstractId = flight.getId();
      std::string flightNumber = flight.getFlightNumber();
      std::string departureDate = flight.getDepartureDate();
      int abstractAircraftId = flight.getAircraftId();
      std::string flightClass = flight.getFlightClass();
      int availableSeats = flight.getAvailableSeats();

      std::cout << "Abstract ID: " << abstractId << "\n";
      std::cout << "Flight Number: " << flightNumber << "\n";
      std::cout << "Departure Date: " << departureDate << "\n";
      std::cout << "Aircraft ID: " << abstractAircraftId << "\n";
      std::cout << "Flight Class: " << flightClass << "\n";
      std::cout << "Available Seats: " << availableSeats << "\n";
      std::cout << "\n";
    }
  }

 private:
  FlightsGateway &flightGateway;
  FlightIdMapper *flightIdMapper;
  AircraftIdMapper *aircraftIdMapper;
  CarrierIdMapper *carrierIdMapper;
};

#endif  // RPBD_FLIGHTMENU_H
