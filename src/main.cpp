#include <iostream>

#include "./gateways/AircraftGateway.h"
#include "./gateways/CarrierGateway.h"
#include "./gateways/ClientGateway.h"
#include "./gateways/EmployeeGateway.h"
#include "./gateways/EmployeeTransfersGateway.h"
#include "./gateways/FlightsGateway.h"
#include "./gateways/HotelGateway.h"
#include "./gateways/RouteGateway.h"
#include "./models/Aircraft.h"
#include "./models/Carriers.h"
#include "./models/EmployeeTransfers.h"
#include "./models/Employees.h"
#include "./utils/AircraftIdMapper.h"
#include "./utils/AircraftMenu.h"
#include "./utils/CarrierIdMapper.h"
#include "./utils/CarrierMenu.h"
#include "./utils/ClientIdMapper.h"
#include "./utils/ClientMenu.h"
#include "./utils/DataBaseConnector.h"
#include "./utils/DataBaseInitializer.h"
#include "./utils/EmployeeIdMapper.h"
#include "./utils/EmployeeMenu.h"
#include "./utils/EmployeeTransferIdMapper.h"
#include "./utils/EmployeeTransferMenu.h"
#include "./utils/FlightMenu.h"
#include "./utils/HotelIdMapper.h"
#include "./utils/HotelMenu.h"
#include "./utils/RouteIdMapper.h"
#include "./utils/RouteMenu.h"

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
      std::cout << "Init database\n";
    } else {
      std::cout << "Init database error\n";
    }

    AircraftIdMapper &aircraftIdMapper = AircraftIdMapper::getInstance();
    CarrierIdMapper &carrierIdMapper = CarrierIdMapper::getInstance();
    FlightIdMapper &flightIdMapper = FlightIdMapper::getInstance();
    HotelIdMapper &hotelIdMapper = HotelIdMapper::getInstance();
    ClientIdMapper &clientIdMapper = ClientIdMapper::getInstance();
    EmployeeIdMapper &employeeIdMapper = EmployeeIdMapper::getInstance();
    EmployeeTransfersIdMapper &transferIdMapper =
        EmployeeTransfersIdMapper::getInstance();
    RouteIdMapper &routeIdMapper = RouteIdMapper::getInstance();

    AircraftGateway aircraftGateway;
    CarrierGateway carrierGateway;
    FlightsGateway flightsGateway;
    HotelGateway hotelGateway;
    ClientGateway clientGateway;
    EmployeeGateway employeeGateway;
    EmployeeTransfersGateway transferGateway;
    RouteGateway routeGateway;

    AircraftMenu aircraftMenu(aircraftGateway);
    CarrierMenu carrierMenu(carrierGateway);
    FlightMenu flightMenu(flightsGateway);
    HotelMenu hotelMenu(hotelGateway);
    ClientMenu clientMenu(clientGateway);
    EmployeeMenu employeeMenu(employeeGateway);
    EmployeeTransferMenu transferMenu(transferGateway);
    RouteMenu routeMenu(routeGateway);

    std::vector<std::tuple<int, std::string>> carrierData;
    if (carrierGateway.getAllCarriers(carrierData)) {
      for (const auto &tuple : carrierData) {
        int realId = std::get<0>(tuple);

        carrierIdMapper.addMapping(carrierIdMapper.generateNextAbstractId(),
                                   realId);
        Carrier carrier(carrierIdMapper.getAbstractId(realId),
                        std::get<1>(tuple));
        carrierIdMapper.carrierVector.push_back(carrier);
      }
    }

    // init aircraft map id
    std::vector<std::tuple<int, std::string, int, std::string, int>>
        aircraftData;
    if (aircraftGateway.getAllAircraft(aircraftData)) {
      for (const auto &tuple : aircraftData) {
        int realId = std::get<0>(tuple);
        aircraftIdMapper.addMapping(aircraftIdMapper.generateNextAbstractId(),
                                    realId);
        Aircraft aircraft(aircraftIdMapper.getAbstractId(realId),
                          std::get<1>(tuple),
                          carrierIdMapper.getAbstractId(std::get<2>(tuple)),
                          std::get<3>(tuple), std::get<4>(tuple));
        aircraftIdMapper.aircraftVector.push_back(aircraft);
      }
    }

    // init flight map id
    std::vector<
        std::tuple<int, std::string, std::string, int, std::string, int>>
        flightData;
    if (flightsGateway.getAllFlights(flightData)) {
      for (const auto &tuple : flightData) {
        int realId = std::get<0>(tuple);

        flightIdMapper.addMapping(flightIdMapper.generateNextAbstractId(),
                                  realId);

        Flights flight(flightIdMapper.getAbstractId(realId), std::get<1>(tuple),
                       std::get<2>(tuple),
                       aircraftIdMapper.getAbstractId(std::get<3>(tuple)),
                       std::get<4>(tuple), std::get<5>(tuple));
        flightIdMapper.flightVector.push_back(flight);
      }
    }

    // init hotel map id
    std::vector<std::tuple<int, std::string, int, std::string>> hotelData;
    if (hotelGateway.getAllHotels(hotelData)) {
      for (const auto &tuple : hotelData) {
        int realId = std::get<0>(tuple);

        hotelIdMapper.addMapping(hotelIdMapper.generateNextAbstractId(),
                                 realId);
        Hotels hotel(hotelIdMapper.getAbstractId(realId), std::get<1>(tuple),
                     std::get<2>(tuple), std::get<3>(tuple));
        hotelIdMapper.hotelVector.push_back(hotel);
      }
    }

    // init client map id
    std::vector<
        std::tuple<int, std::string, std::string, std::string, std::string>>
        clientData;
    if (clientGateway.getAllClients(clientData)) {
      for (const auto &tuple : clientData) {
        int realId = std::get<0>(tuple);

        clientIdMapper.addMapping(clientIdMapper.generateNextAbstractId(),
                                  realId);
        Clients client(clientIdMapper.getAbstractId(realId), std::get<1>(tuple),
                       std::get<2>(tuple), std::get<3>(tuple),
                       std::get<4>(tuple));
        clientIdMapper.clientVector.push_back(client);
      }
    }

    // init employee map id
    std::vector<std::tuple<int, std::string, std::string, std::string,
                           std::string, double>>
        employeeData;
    if (employeeGateway.getAllEmployees(employeeData)) {
      for (const auto &tuple : employeeData) {
        int realId = std::get<0>(tuple);

        employeeIdMapper.addMapping(employeeIdMapper.generateNextAbstractId(),
                                    realId);
        Employees employee(employeeIdMapper.getAbstractId(realId),
                           std::get<1>(tuple), std::get<2>(tuple),
                           std::get<3>(tuple), std::get<4>(tuple),
                           std::get<5>(tuple));
        employeeIdMapper.employeeVector.push_back(employee);
      }
    }

    // init transfer map id
    std::vector<std::tuple<int, int, std::string, std::string, std::string,
                           std::string>>
        transferData;
    if (transferGateway.getAllTransfers(transferData)) {
      for (const auto &tuple : transferData) {
        int realId = std::get<0>(tuple);
        std::cout << realId << " real id\n";
        transferIdMapper.addMapping(transferIdMapper.generateNextAbstractId(),
                                    realId);
        EmployeeTransfers transfer(
            transferIdMapper.getAbstractId(realId),
            employeeIdMapper.getAbstractId(std::get<1>(tuple)),
            std::get<2>(tuple), std::get<3>(tuple), std::get<4>(tuple),
            std::get<5>(tuple));
        transferIdMapper.employeeTransfersVector.push_back(transfer);
      }
    }

    // init route map id
    std::vector<std::tuple<int, std::string, std::string, std::string, int, int,
                           int, int, std::string, std::string>>
        routeData;
    if (routeGateway.getAllRoutes(routeData)) {
      for (const auto &tuple : routeData) {
        int realId = std::get<0>(tuple);
        std::cout << realId << " real id\n";
        routeIdMapper.addMapping(routeIdMapper.generateNextAbstractId(),
                                 realId);
        Route route(routeIdMapper.getAbstractId(realId), std::get<1>(tuple),
                    std::get<2>(tuple), std::get<3>(tuple), std::get<4>(tuple),
                    hotelIdMapper.getAbstractId(std::get<5>(tuple)),
                    flightIdMapper.getAbstractId(std::get<6>(tuple)),
                    employeeIdMapper.getAbstractId(std::get<7>(tuple)),
                    std::get<8>(tuple), std::get<9>(tuple));
        routeIdMapper.routeVector.push_back(route);
      }
    }

    int choice;
    do {
      std::cout << "\n\nMain Menu:\n";
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
      std::cout << "Enter your choice: ";
      std::cin >> choice;

      switch (choice) {
        case 1:
          aircraftMenu.displayMenu();
          break;
        case 2:
          carrierMenu.displayMenu();
          break;
        case 4:
          clientMenu.displayMenu();
          break;
        case 5:
          transferMenu.displayMenu();
          break;
        case 6:
          employeeMenu.displayMenu();
          break;
        case 7:
          flightMenu.displayMenu();
          break;
        case 8:
          hotelMenu.displayMenu();
          break;
        case 9:
          routeMenu.displayMenu();
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
