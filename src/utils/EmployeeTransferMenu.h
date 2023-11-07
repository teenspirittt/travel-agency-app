#ifndef RPBD_EMPLOYEETRANSFERMENU_H
#define RPBD_EMPLOYEETRANSFERMENU_H

#include "../gateways/EmployeeGateway.h"
#include "../gateways/EmployeeTransfersGateway.h"
#include "../models/EmployeeTransfers.h"
#include "../utils/EmployeeMenu.h"
#include "EmployeeTransferIdMapper.h"

class EmployeeTransferMenu {
 public:
  explicit EmployeeTransferMenu(EmployeeTransfersGateway& etGateway,
                                EmployeeGateway& eGateway)
      : employeeTransferGateway(etGateway), employeeGateway(eGateway) {
    employeeTransfersIdMapper = &EmployeeTransfersIdMapper::getInstance();
    employeeIdMapper = &EmployeeIdMapper::getInstance();
  }
  void displayMenu() {
    int choice;
    do {
      std::cout << "\nEmployee Transfer Menu:\n";
      std::cout << "1. Insert Employee Transfer\n";
      std::cout << "2. Delete Employee Transfer\n";
      std::cout << "3. Update Employee Transfer\n";
      std::cout << "4. View All Employee Transfers\n";
      std::cout << "5. Exit\n";
      std::cout << "Enter your choice: ";
      std::cin >> choice;

      switch (choice) {
        case 1:
          insertEmployeeTransfer();
          break;
        case 2:
          deleteEmployeeTransfer();
          break;
        case 3:
          updateEmployeeTransfer();
          break;
        case 4:
          viewAllEmployeeTransfers();
          break;
        case 5:
          std::cout << "Exiting Employee Transfer Menu.\n";
          break;
        default:
          std::cout << "Invalid choice. Please try again.\n";
      }
    } while (choice != 5);
  }

  void insertEmployeeTransfer() {
    int employeeId;
    std::string newPosition, transferReason, orderNumber, orderDate;

    EmployeeMenu::displayAllEmployees(*employeeIdMapper);
    std::cout << "Insert employee ID: (required int)\n";
    std::cin >> employeeId;

    if (!EmployeeMenu::isEmployeeIdValid(employeeId, *employeeIdMapper)) {
      std::cerr << "Employee " << employeeId << " does not exist.\n";
      if (EmployeeMenu::displayAllEmployees(*employeeIdMapper)) {
        std::cout << "Please choose an existing employee ID.\n";
      } else {
        std::cerr << "No employees available. Cannot proceed.\n";
        return;
      }
    }

    std::cout << "Insert new position: (required string)\n";
    std::cin.ignore();
    std::getline(std::cin, newPosition);

    std::cout << "Insert transfer reason: (required string)\n";
    std::getline(std::cin, transferReason);

    std::cout << "Insert order number: (required string)\n";
    std::getline(std::cin, orderNumber);

    std::cout << "Insert order date: (required string)\n";
    std::cin >> orderDate;

    EmployeeTransfers employeeTransfer(
        employeeTransfersIdMapper->generateNextAbstractId(), employeeId,
        newPosition, transferReason, orderNumber, orderDate);

    if (employeeTransferGateway.insertTransfer(
            employeeIdMapper->getRealId(employeeId), newPosition,
            transferReason, orderNumber, orderDate)) {
      std::cout << "Employee transfer inserted successfully. "
                << employeeTransfer.getId() << "\n";
      employeeTransfersIdMapper->employeeTransfersVector.push_back(
          employeeTransfer);
      employeeTransfersIdMapper->addMapping(
          employeeTransfer.getId(),
          employeeTransferGateway.getLastInsertedId());
    } else {
      std::cerr << "Failed to insert employee transfer.\n";
    }
  }

  void deleteEmployeeTransfer() {
    int abstractId;
    std::cout << "Insert ID to delete\n";
    std::cin >> abstractId;

    for (auto it = employeeTransfersIdMapper->employeeTransfersVector.begin();
         it != employeeTransfersIdMapper->employeeTransfersVector.end(); ++it) {
      if (it->getId() == abstractId) {
        employeeTransfersIdMapper->employeeTransfersVector.erase(it);
        break;
      }
    }

    if (employeeTransferGateway.deleteTransfer(
            employeeTransfersIdMapper->getRealId(abstractId))) {
      std::cout << "Employee transfer with abstract ID " << abstractId
                << " deleted successfully.\n";
    } else {
      std::cerr << "Failed to delete employee transfer with abstract ID "
                << abstractId << ".\n";
    }
  }

  void viewAllEmployeeTransfers() {
    std::cout << "\nAll Employee Transfers:\n";

    for (const EmployeeTransfers& employeeTransfer :
         employeeTransfersIdMapper->employeeTransfersVector) {
      int abstractId = employeeTransfer.getId();
      int employeeId = employeeTransfer.getEmployeeId();
      std::string newPosition = employeeTransfer.getNewPosition();
      std::string transferReason = employeeTransfer.getTransferReason();
      std::string orderNumber = employeeTransfer.getOrderNumber();
      std::string orderDate = employeeTransfer.getOrderDate();

      std::string fullName;
      std::string address;
      std::string dateOfBirth;
      std::string position;
      double salary;

      employeeGateway.getEmployee(employeeIdMapper->getRealId(employeeId),
                                  fullName, address, dateOfBirth, position,
                                  salary);

      std::cout << "#" << abstractId << "\n";
      std::cout << "Employee name: " << fullName << "\n";
      std::cout << "New Position: " << newPosition << "\n";
      std::cout << "Transfer Reason: " << transferReason << "\n";
      std::cout << "Order Number: " << orderNumber << "\n";
      std::cout << "Order Date: " << orderDate << "\n";
      std::cout << "\n";
    }
  }

  void updateEmployeeTransfer() {
    int abstractId;
    std::cout << "Enter the number of the employee transfer you want to "
                 "update: \n";
    std::cin >> abstractId;

    if (!employeeTransfersIdMapper->getRealId(abstractId)) {
      std::cerr << "Employee #" << abstractId << " does not exist.\n";
      return;
    }

    int realEmployeeTransferId =
        employeeTransfersIdMapper->getRealId(abstractId);
    int newEmployeeId;
    std::string newPosition, transferReason, orderNumber, orderDate;

    std::cout
        << "Enter new employee number (or press Enter to keep the current "
           "value): \n";
    std::cin >> newEmployeeId;

    if (!EmployeeMenu::isEmployeeIdValid(newEmployeeId, *employeeIdMapper)) {
      std::cerr << "Employee " << newEmployeeId << " does not exist.\n";
      if (EmployeeMenu::displayAllEmployees(*employeeIdMapper)) {
        std::cout << "Please choose an existing employee ID.\n";
        return;
      } else {
        std::cerr << "No employees available. Cannot proceed.\n";
        return;
      }
    }

    std::cout
        << "Enter new position (or press Enter to keep the current value): \n";
    std::cin.ignore();
    std::getline(std::cin, newPosition);

    std::cout << "Enter new transfer reason (or press Enter to keep the "
                 "current value): \n";
    std::getline(std::cin, transferReason);

    std::cout << "Enter new order number (or press Enter to keep the current "
                 "value): \n";
    std::getline(std::cin, orderNumber);

    std::cout << "Enter new order date (or press Enter to keep the current "
                 "value): \n";
    std::cin >> orderDate;

    for (EmployeeTransfers& employeeTransfer :
         employeeTransfersIdMapper->employeeTransfersVector) {
      if (employeeTransfer.getId() ==
          employeeTransfersIdMapper->getAbstractId(realEmployeeTransferId)) {
        if (newEmployeeId != 0) {
          employeeTransfer.setEmployeeId(newEmployeeId);
        } else {
          newEmployeeId = employeeTransfer.getEmployeeId();
        }
        if (!newPosition.empty()) {
          employeeTransfer.setNewPosition(newPosition);
        } else {
          newPosition = employeeTransfer.getNewPosition();
        }
        if (!transferReason.empty()) {
          employeeTransfer.setTransferReason(transferReason);
        } else {
          transferReason = employeeTransfer.getTransferReason();
        }
        if (!orderNumber.empty()) {
          employeeTransfer.setOrderNumber(orderNumber);
        } else {
          orderNumber = employeeTransfer.getOrderNumber();
        }
        if (!orderDate.empty()) {
          employeeTransfer.setOrderDate(orderDate);
        } else {
          orderDate = employeeTransfer.getOrderDate();
        }
        break;
      }
    }

    if (employeeTransferGateway.updateTransfer(
            realEmployeeTransferId, employeeIdMapper->getRealId(newEmployeeId),
            newPosition, transferReason, orderNumber, orderDate)) {
      std::cout << "Employee transfer #" << abstractId
                << " updated successfully.\n";
    } else {
      std::cerr << "Failed to update employee #" << abstractId << ".\n";
    }
  }

 private:
  EmployeeTransfersGateway& employeeTransferGateway;
  EmployeeTransfersIdMapper* employeeTransfersIdMapper;
  EmployeeIdMapper* employeeIdMapper;
  EmployeeGateway& employeeGateway;
};

#endif  // RPBD_EMPLOYEETRANSFERMENU_H