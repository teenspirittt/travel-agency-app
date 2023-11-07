#ifndef RPBD_CLIENTMENU_H
#define RPBD_CLIENTMENU_H

#include "../gateways/ClientGateway.h"
#include "../models/Clients.h"
#include "ClientIdMapper.h"

class ClientMenu {
 public:
  explicit ClientMenu(ClientGateway &cGateway) : clientGateway(cGateway) {
    clientIdMapper = &ClientIdMapper::getInstance();
  }

  void displayMenu() {
    int choice;
    do {
      std::cout << "\nClient Menu:\n";
      std::cout << "1. Insert Client\n";
      std::cout << "2. Delete Client\n";
      std::cout << "3. Update Client\n";
      std::cout << "4. View All Clients\n";
      std::cout << "5. Exit\n";
      std::cout << "Enter your choice: ";
      std::cin >> choice;

      switch (choice) {
        case 1:
          insertClient();
          break;
        case 2:
          deleteClient();
          break;
        case 3:
          updateClient();
          break;
        case 4:
          viewAllClients();
          break;
        case 5:
          std::cout << "Exiting Client Menu.\n";
          break;
        default:
          std::cout << "Invalid choice. Please try again.\n";
      }
    } while (choice != 5);
  }

  void insertClient() {
    std::string fullName, phone, orderDate, seat;

    std::cout << "Insert full name: (required string)\n";
    std::cin.ignore();
    std::getline(std::cin, fullName);

    std::cout << "Insert phone: (required string)\n";
    std::getline(std::cin, phone);

    std::cout << "Insert order date: (required string)\n";
    std::getline(std::cin, orderDate);

    std::cout << "Insert seat: (required string)\n";
    std::getline(std::cin, seat);

    if (clientGateway.insertClient(fullName, phone, orderDate, seat)) {
      std::cout << "Client inserted successfully.\n";

      Clients newClient(clientIdMapper->generateNextAbstractId(), fullName,
                        phone, orderDate, seat);
      clientIdMapper->clientVector.push_back(newClient);
      clientIdMapper->addMapping(newClient.getId(),
                                 clientGateway.getLastInsertedId());
    }
  }

  void deleteClient() {
    int abstractId;
    std::cout << "Insert ID to delete\n";
    std::cin >> abstractId;

    if (clientGateway.deleteClient(clientIdMapper->getRealId(abstractId))) {
      std::cout << "Client with abstract ID " << abstractId
                << " deleted successfully.\n";
      for (auto it = clientIdMapper->clientVector.begin();
           it != clientIdMapper->clientVector.end(); ++it) {
        if (it->getId() == abstractId) {
          clientIdMapper->clientVector.erase(it);
          break;
        }
      }
    } else {
      std::cerr << "Failed to delete client with abstract ID " << abstractId
                << ".\n";
    }
  }

  void viewAllClients() {
    std::cout << "\nAll Clients:\n";

    for (const Clients &client : clientIdMapper->clientVector) {
      int abstractId = client.getId();
      std::string fullName = client.getFullName();
      std::string phone = client.getPhone();
      std::string orderDate = client.getOrderDate();
      std::string seat = client.getSeat();

      std::cout << "Abstract ID: " << abstractId << "\n";
      std::cout << "Full Name: " << fullName << "\n";
      std::cout << "Phone: " << phone << "\n";
      std::cout << "Order Date: " << orderDate << "\n";
      std::cout << "Seat: " << seat << "\n";
      std::cout << "\n";
    }
  }

  void updateClient() {
    int abstractId;
    std::cout << "Enter the ID of the client you want to update: ";
    std::cin >> abstractId;

    if (!clientIdMapper->getRealId(abstractId)) {
      std::cerr << "Client with abstract ID " << abstractId
                << " does not exist.\n";
      return;
    }

    int realClientId = clientIdMapper->getRealId(abstractId);
    std::string newFullName, newPhone, newOrderDate, newSeat;

    std::cout
        << "Enter new full name (or press Enter to keep the current value): ";
    std::cin.ignore();
    std::getline(std::cin, newFullName);

    std::cout << "Enter new phone (or press Enter to keep the current value): ";
    std::getline(std::cin, newPhone);

    std::cout
        << "Enter new order date (or press Enter to keep the current value): ";
    std::getline(std::cin, newOrderDate);

    std::cout << "Enter new seat (or press Enter to keep the current value): ";
    std::getline(std::cin, newSeat);

    for (Clients &client : clientIdMapper->clientVector) {
      if (client.getId() == clientIdMapper->getAbstractId(realClientId)) {
        if (!newFullName.empty()) {
          client.setFullName(newFullName);
        } else {
          newFullName = client.getFullName();
        }
        if (!newPhone.empty()) {
          client.setPhone(newPhone);
        } else {
          newPhone = client.getPhone();
        }
        if (!newOrderDate.empty()) {
          client.setOrderDate(newOrderDate);
        } else {
          newOrderDate = client.getOrderDate();
        }
        if (!newSeat.empty()) {
          client.setSeat(newSeat);
        } else {
          newSeat = client.getSeat();
        }
        break;
      }
    }

    if (clientGateway.updateClient(realClientId, newFullName, newPhone,
                                   newOrderDate, newSeat)) {
      std::cout << "Client with abstract ID " << abstractId
                << " updated successfully.\n";
    } else {
      std::cerr << "Failed to update client with abstract ID " << abstractId
                << ".\n";
    }
  }

  static bool isClientIdValid(int clientId, const ClientIdMapper &idMapper) {
    for (const Clients &client : idMapper.clientVector) {
      if (client.getId() == clientId) {
        return true;
      }
    }
    return false;
  }

  static bool displayAllClients(const ClientIdMapper &idMapper) {
    if (idMapper.clientVector.empty()) {
      std::cerr << "No clients available. Cannot proceed.\n";
      return false;
    }

    std::cout << "Available Clients:\n";
    for (const Clients &client : idMapper.clientVector) {
      std::cout << "{id=" << client.getId() << "; name=\""
                << client.getFullName() << "\"}\n";
    }
    return true;
  }

 private:
  ClientGateway &clientGateway;
  ClientIdMapper *clientIdMapper;
};

#endif  // RPBD_CLIENTMENU_H