#pragma once

#include "../gateways/AircraftGateway.h"
#include "../gateways/CarrierGateway.h"
#include "../models/Aircraft.h"
#include "AircraftIdMapper.h"
#include "CarrierIdMapper.h"
#include "CarrierMenu.h"

class AircraftMenu {
public:
    explicit AircraftMenu(AircraftGateway &aGateway)
            : aircraftGateway(aGateway) {
        aircraftIdMapper = &AircraftIdMapper::getInstance();
        carrierIdMapper = &CarrierIdMapper::getInstance();
    }

    void displayMenu() {
        int choice;
        do {
            std::cout << "\nAircraft Menu:\n";
            std::cout << "1. Insert Aircraft\n";
            std::cout << "2. Delete Aircraft\n";
            std::cout << "3. Update Aircraft\n";
            std::cout << "4. View All Aircraft\n";
            std::cout << "5. Exit\n";
            std::cout << "Enter your choice: \n";
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
        } while (choice != 5);
    }

    void insertAircraft() {
        std::string aircraftType, manufacturer;
        int carrierId, capacity;

        std::cout << "Insert aircraft type: (required string)\n";
        std::cin.ignore();
        std::getline(std::cin, aircraftType);
        std::cout << "Insert manufacturer: (required string)\n";
        std::getline(std::cin, manufacturer);

        std::cout << "Insert carrier number: (required int)\n";
        CarrierMenu::displayAllCarriers(*carrierIdMapper);
        std::cin >> carrierId;

        if (!CarrierMenu::isCarrierIdValid(carrierId, *carrierIdMapper)) {
            std::cerr << "Carrier " << carrierId << " does not exist.\n";
            if (CarrierMenu::displayAllCarriers(*carrierIdMapper)) {
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
            aircraftIdMapper->aircraftVector.push_back(aircraft);
            aircraftIdMapper->addMapping(aircraft.getId(),
                                         aircraftGateway.getLastInsertedId());
        } else {
            std::cerr << "Failed to insert aircraft.\n";
        }
    }

    void deleteAircraft() {
        int abstractId;
        std::cout << "Insert id to delete\n";
        std::cin >> abstractId;

        for (auto it = aircraftIdMapper->aircraftVector.begin();
             it != aircraftIdMapper->aircraftVector.end(); ++it) {
            if (it->getId() == abstractId) {
                aircraftIdMapper->aircraftVector.erase(it);
                break;
            }
        }

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
        std::cout << "\nAll Aircraft:\n";

        for (const Aircraft &aircraft: aircraftIdMapper->aircraftVector) {
            int abstractId = aircraft.getId();
            std::string aircraftType = aircraft.getAircraftType();
            int abstractCarrierId = aircraft.getCarrierId();
            std::string manufacturer = aircraft.getManufacturer();
            int capacity = aircraft.getCapacity();

            std::cout << "Abstract ID: " << abstractId << "\n";
            std::cout << "Aircraft Type: " << aircraftType << "\n";
            std::cout << "Carrier ID: " << abstractCarrierId << "\n";
            std::cout << "Manufacturer: " << manufacturer << "\n";
            std::cout << "Capacity: " << capacity << "\n";
            std::cout << "\n";
        }
    }

    void updateAircraft() {
        int abstractId;
        std::cout << "Enter the abstract ID of the aircraft you want to update: \n";
        std::cin >> abstractId;

        if (!aircraftIdMapper->getRealId(abstractId)) {
            std::cerr << "Aircraft with abstract ID " << abstractId
                      << " does not exist.\n";
            return;
        }

        int realAircraftId = aircraftIdMapper->getRealId(abstractId);
        std::cout << realAircraftId;
        std::string newAircraftType, newManufacturer;
        int newCarrierId, newCapacity;

        std::cout << "Enter new aircraft type (or press Enter to keep the current "
                     "value): \n";
        std::cin.ignore();
        std::getline(std::cin, newAircraftType);

        std::cout << "Enter new manufacturer (or press Enter to keep the current "
                     "value): \n";
        std::getline(std::cin, newManufacturer);
        std::cout
                << "Enter new capacity (or press Enter to keep the current value): \n";
        std::cin >> newCapacity;
        std::cout << "Enter new carrier ID (or press Enter to keep the current "
                     "value): \n";
        std::cin >> newCarrierId;

        for (Aircraft &aircraft: aircraftIdMapper->aircraftVector) {
            if (aircraft.getId() == aircraftIdMapper->getAbstractId(realAircraftId)) {
                if (!newAircraftType.empty()) {
                    aircraft.setAircraftType(newAircraftType);
                } else {
                    newAircraftType = aircraft.getAircraftType();
                }
                if (newCarrierId != 0) {
                    aircraft.setCarrierId(newCarrierId);
                } else {
                    newCarrierId = aircraft.getCarrierId();
                }
                if (!newManufacturer.empty()) {
                    aircraft.setManufacturer(newManufacturer);
                } else {
                    newManufacturer = aircraft.getManufacturer();
                }
                if (newCapacity != 0) {
                    aircraft.setCapacity(newCapacity);
                } else {
                    newCapacity = aircraft.getCapacity();
                }
                break;
            }
        }

        if (aircraftGateway.updateAircraft(realAircraftId, newAircraftType,
                                           newCarrierId, newManufacturer,
                                           newCapacity)) {
            std::cout << "Aircraft with abstract ID " << abstractId
                      << " updated successfully.\n";
        } else {
            std::cerr << "Failed to update aircraft with abstract ID " << abstractId
                      << ".\n";
        }
    }

    static bool isAircraftIdValid(int aircraftId, const AircraftIdMapper &idMapper) {
        for (const Aircraft &aircraft: idMapper.aircraftVector) {
            if (aircraft.getId() == aircraftId) {
                return true;
            }
        }
        return false;
    }

    static bool displayAllAircraft(const AircraftIdMapper &idMapper) {
        if (idMapper.aircraftVector.empty()) {
            std::cerr << "No aircraft available. Cannot proceed.\n";
            return false;
        }

        std::cout << "Available Aircraft:\n";
        for (const Aircraft &aircraft: idMapper.aircraftVector) {
            std::cout << "Abstract ID: " << aircraft.getId() << "\n";
            std::cout << "Aircraft Type: " << aircraft.getAircraftType() << "\n";
            std::cout << "Carrier ID: " << aircraft.getCarrierId() << "\n";
            std::cout << "Manufacturer: " << aircraft.getManufacturer() << "\n";
            std::cout << "Capacity: " << aircraft.getCapacity() << "\n";
            std::cout << "\n";
        }
        return true;
    }

private:
    AircraftGateway &aircraftGateway;
    AircraftIdMapper *aircraftIdMapper;
    CarrierIdMapper *carrierIdMapper;

};
