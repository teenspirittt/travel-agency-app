#include <iostream>

#include "./gateways/AircraftGateway.h"
#include "./gateways/CarrierGateway.h"
#include "./gateways/ClientGateway.h"
#include "./gateways/EmployeeGateway.h"
#include "./gateways/EmployeeTransfers.h"
#include "./gateways/FlightsGateway.h"
#include "./gateways/HotelGateway.h"
#include "./models/Aircraft.h"
#include "./utils/AircraftIdMapper.h"
#include "./utils/AircraftMenu.h"
#include "./utils/CarrierIdMapper.h"
#include "./utils/DataBaseConnector.h"
#include "./utils/DataBaseInitializer.h"
#include "./utils/CarrierMenu.h"
#include "./models/Carriers.h"

int main() {
    DataBaseConnector &dbConnector = DataBaseConnector::getInstance();

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
            std::cout << "Init  database error";
        }

        AircraftIdMapper &aircraftIdMapper = AircraftIdMapper::getInstance();
        CarrierIdMapper &carrierIdMapper = CarrierIdMapper::getInstance();

        AircraftGateway aircraftGateway;
        CarrierGateway carrierGateway;

        AircraftMenu aircraftMenu(aircraftGateway, carrierGateway);
        CarrierMenu carrierMenu(carrierGateway);

        std::vector<std::tuple<int, std::string, int, std::string, int>>
                aircraftData;
        if (aircraftGateway.getAllAircraft(aircraftData)) {
            for (const auto &tuple: aircraftData) {
                int realId = std::get<0>(tuple);
                aircraftIdMapper.addMapping(aircraftIdMapper.generateNextAbstractId(),
                                            realId);
                Aircraft aircraft(aircraftIdMapper.getAbstractId(realId),
                                  std::get<1>(tuple), std::get<2>(tuple),
                                  std::get<3>(tuple), std::get<4>(tuple));
                aircraftIdMapper.aircraftVector.push_back(aircraft);
            }
        }


        std::vector<std::tuple<int, std::string>> carrierData;
        if (carrierGateway.getAllCarriers(carrierData)) {
            for (const auto &tuple: carrierData) {
                int realId = std::get<0>(tuple);

                carrierIdMapper.addMapping(carrierIdMapper.generateNextAbstractId(),
                                           realId);
                Carrier carrier(carrierIdMapper.getAbstractId(realId), std::get<1>(tuple));
                carrierIdMapper.carrierVector.push_back(carrier);
            }
        }

        int choice;
        do {
            std::cout << "\n\nMain  Menu:\n";
            std::cout << "1. Aircraft Menu\n";
            std::cout << "2. Carrier Menu\n";
            std::cout << "3. Client Route Menu\n";
            std::cout << "4. Client Menu\n";
            std::cout << "5. Employee Transfer Menu\n";
            std::cout << "6. Employee Menu\n";
            std::cout << "7. Flight Menu\n";
            std::cout << "8. Hotel Menu\n";
            std::cout << "9. Route  Menu\n";
            std::cout << "10. Exit\n";
            std::cout << "Enter  your choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1:
                    aircraftMenu.displayMenu();
                    break;
                case 2:
                    carrierMenu.displayMenu();
                    break;
                case 10:
                    std::cout << "Exiting the program.\n";
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 10);

        dbConnector.disconnect();
    } else {
        std::cerr << "Failed  to  connect to the database." << std::endl;
        return 0;
    }
}
