#ifndef RPBD_CLIENTROUTEMENU_H
#define RPBD_CLIENTROUTEMENU_H

#include <algorithm>

#include "../gateways/ClientRouteGateway.h"
#include "../models/ClientRoute.h"
#include "ClientIdMapper.h"
#include "ClientMenu.h"
#include "RouteIdMapper.h"
#include "RouteMenu.h"

class ClientRouteMenu {
 public:
  explicit ClientRouteMenu(ClientRouteGateway& crGateway,
                           std::vector<ClientRoute> clientRouteObjects)
      : clientRouteGateway(crGateway), clientRouteObjects(clientRouteObjects) {
    clientIdMapper = &ClientIdMapper::getInstance();
    routeIdMapper = &RouteIdMapper::getInstance();
  }
  void displayMenu() {
    int choice;
    do {
      std::cout << "\ClientRoute Menu:\n";
      std::cout << "1. Insert ClientRoute\n";
      std::cout << "2. Delete ClientRoute\n";
      std::cout << "3. Update ClientRoute\n";
      std::cout << "4. View All ClientRoute\n";
      std::cout << "5. Exit\n";
      std::cout << "Enter your choice: \n";
      std::cin >> choice;

      switch (choice) {
        case 1:
          insertClientRoute();
          break;
        case 2:
          deleteClientRoute();
          break;
        case 3:
          updateClientRoute();
          break;
        case 4:
          viewAllClientRoutes();
          break;
        case 5:
          std::cout << "Exiting ClientRoute Menu.\n";
          break;
        default:
          std::cout << "Invalid choice. Please try again.\n";
      }
    } while (choice != 5);
  }

  void insertClientRoute() {
    int clientId, routeId;

    ClientMenu::displayAllClients(*clientIdMapper);
    std::cout << "Enter the number of the client: ";
    std::cin >> clientId;

    if (!ClientMenu::isClientIdValid(clientId, *clientIdMapper)) {
      std::cerr << "Client #" << clientId << " does not exist.\n";
      return;
    }

    RouteMenu::displayAllRoutes(*routeIdMapper);
    std::cout << "Enter the number of the route: ";
    std::cin >> routeId;

    if (!RouteMenu::isRouteIdValid(routeId, *routeIdMapper)) {
      std::cerr << "Route #" << routeId << " does not exist.\n";
      return;
    }

    ClientRoute clientRoute(clientId, routeId);
    clientRouteObjects.push_back(clientRoute);

    if (clientRouteGateway.insertClientRoute(
            clientIdMapper->getRealId(clientId),
            routeIdMapper->getRealId(routeId))) {
      std::cout << "Client-Route relationship inserted successfully.\n";
    } else {
      std::cerr << "Failed to insert Client-Route relationship.\n";
    }
  }

  void deleteClientRoute() {
    int clientId, routeId;
    std::cout << "Enter the number of the client: ";
    std::cin >> clientId;

    std::cout << "Enter the number of the route: ";
    std::cin >> routeId;

    if (clientRouteGateway.deleteClientRoute(
            clientIdMapper->getRealId(clientId),
            routeIdMapper->getRealId(routeId))) {
      std::cout << "Client-Route relationship deleted successfully.\n";

      auto it =
          std::remove_if(clientRouteObjects.begin(), clientRouteObjects.end(),
                         [clientId, routeId](const ClientRoute& clientRoute) {
                           return clientRoute.getClientId() == clientId &&
                                  clientRoute.getRouteId() == routeId;
                         });
    } else {
      std::cerr << "Failed to delete Client-Route relationship.\n";
    }
  }

  void updateClientRoute() {
    int clientId, routeId;
    std::cout << "Enter the number of the client: ";
    std::cin >> clientId;

    std::cout << "Enter the number of the route: ";
    std::cin >> routeId;

    if (!clientIdMapper->getRealId(clientId) ||
        !routeIdMapper->getRealId(routeId)) {
      std::cerr << "Client or Route with given numbers does not exist.\n";
      return;
    }
    int newClientId, newRouteId;
    std::cout << "Enter the number of the client: ";
    std::cin >> newClientId;

    std::cout << "Enter the new ID of the client: ";
    std::cin >> newRouteId;

    for (ClientRoute& croute : clientRouteObjects) {
      if (croute.getClientId() == clientId) {
        if ((clientId - newClientId) != 0) {
          croute.setClientId(newClientId);
        } else {
          newClientId = clientId;
        }
      }

      if (croute.getRouteId() == routeId) {
        if ((routeId - newRouteId) != 0) {
          croute.setRouteId(newRouteId);
        } else {
          newRouteId = routeId;
        }
      }
    }

    if (clientRouteGateway.updateClientRoute(clientId, routeId, newClientId,
                                             newRouteId)) {
      std::cout << "Client Route with route number = " << routeId
                << "and client number = " << clientId
                << " updated successfully.\n";
    } else {
      std::cerr << "Failed to update client route\n";
    }
  }

  void viewAllClientRoutes() {
    if (clientRouteObjects.empty()) {
      std::cerr << "No client-route relationships available.\n";
      return;
    }

    std::cout << "All Client-Route Relationships:\n";

    for (const ClientRoute& clientRoute : clientRouteObjects) {
      int clientId = clientRoute.getClientId();
      int routeId = clientRoute.getRouteId();

      std::cout << "Client #: " << clientId << "\n";
      std::cout << "Route #: " << routeId << "\n";
      std::cout << "\n";
    }
  }

 private:
  ClientRouteGateway& clientRouteGateway;
  ClientIdMapper* clientIdMapper;
  std::vector<ClientRoute> clientRouteObjects;
  RouteIdMapper* routeIdMapper;
};
#endif  // RPBD_CLIENTROUTEMENU_H