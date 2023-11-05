#ifndef RPBD_EMPLOYEEMENU_H
#define RPBD_EMPLOYEEMENU_H

#include "../gateways/EmployeeGateway.h"
#include "../models/Employees.h"
#include "EmployeeIdMapper.h"

class EmployeeMenu {
 public:
  explicit EmployeeMenu(EmployeeGateway& eGateway) : employeeGateway(eGateway) {
    employeeIdMapper = &EmployeeIdMapper::getInstance();
  }

  void displayMenu() {
    int choice;
    do {
      std::cout << "\nEmployee Menu:\n";
      std::cout << "1. Insert Employee\n";
      std::cout << "2. Delete Employee\n";
      std::cout << "3. Update Employee\n";
      std::cout << "4. View All Employees\n";
      std::cout << "5. Exit\n";
      std::cout << "Enter your choice: ";
      std::cin >> choice;

      switch (choice) {
        case 1:
          insertEmployee();
          break;
        case 2:
          deleteEmployee();
          break;
        case 3:
          updateEmployee();
          break;
        case 4:
          viewAllEmployees();
          break;
        case 5:
          std::cout << "Exiting Client Menu.\n";
          break;
        default:
          std::cout << "Invalid choice. Please try again.\n";
      }
    } while (choice != 5);
  }

  void insertEmployee() {
    std::string fullName, address, dateOfBirth, position;
    double salary;

    std::cout << "Insert full name: (required string)\n";
    std::cin.ignore();
    std::getline(std::cin, fullName);

    std::cout << "Insert address: (required string)\n";
    std::getline(std::cin, address);

    std::cout << "Insert date of birth: (required string)\n";
    std::cin >> dateOfBirth;

    std::cout << "Insert position: (required string)\n";
    std::cin.ignore();
    std::getline(std::cin, position);

    std::cout << "Insert salary: (required double)\n";
    std::cin >> salary;

    Employees employee(employeeIdMapper->generateNextAbstractId(), fullName,
                       address, dateOfBirth, position, salary);

    if (employeeGateway.insertEmployee(fullName, address, dateOfBirth, position,
                                       salary)) {
      std::cout << "Employee inserted successfully.\n";
      employeeIdMapper->employeeVector.push_back(employee);
      employeeIdMapper->addMapping(employee.getId(),
                                   employeeGateway.getLastInsertedId());
    } else {
      std::cerr << "Failed to insert employee.\n";
    }
  }

  void deleteEmployee() {
    int abstractId;
    std::cout << "Insert ID to delete\n";
    std::cin >> abstractId;

    for (auto it = employeeIdMapper->employeeVector.begin();
         it != employeeIdMapper->employeeVector.end(); ++it) {
      if (it->getId() == abstractId) {
        employeeIdMapper->employeeVector.erase(it);
        break;
      }
    }

    if (employeeGateway.deleteEmployee(
            employeeIdMapper->getRealId(abstractId))) {
      std::cout << "Employee with abstract ID " << abstractId
                << " deleted successfully.\n";
    } else {
      std::cerr << "Failed to delete employee with abstract ID " << abstractId
                << ".\n";
    }
  }

  void viewAllEmployees() {
    std::cout << "\nAll Employees:\n";

    for (const Employees& employee : employeeIdMapper->employeeVector) {
      int abstractId = employee.getId();
      std::string fullName = employee.getFullName();
      std::string address = employee.getAddress();
      std::string dateOfBirth = employee.getDateOfBirth();
      std::string position = employee.getPosition();
      double salary = employee.getSalary();

      std::cout << "Abstract ID: " << abstractId << "\n";
      std::cout << "Full Name: " << fullName << "\n";
      std::cout << "Address: " << address << "\n";
      std::cout << "Date of Birth: " << dateOfBirth << "\n";
      std::cout << "Position: " << position << "\n";
      std::cout << "Salary: " << salary << "\n";
      std::cout << "\n";
    }
  }

  static bool isEmployeeIdValid(int employeeId,
                                const EmployeeIdMapper& idMapper) {
    for (const Employees& employee : idMapper.employeeVector) {
      if (employee.getId() == employeeId) {
        return true;
      }
    }
    return false;
  }

  static bool displayAllEmployees(const EmployeeIdMapper& idMapper) {
    if (idMapper.employeeVector.empty()) {
      std::cerr << "No employees available. Cannot proceed.\n";
      return false;
    }

    std::cout << "Available Employees:\n";
    for (const Employees& employee : idMapper.employeeVector) {
      std::cout << "{id=" << employee.getId() << "; name=\""
                << employee.getFullName() << "\"}\n";
    }
    return true;
  }

  void updateEmployee() {
    int abstractId;
    std::cout << "Enter the ID of the employee you want to update: ";
    std::cin >> abstractId;

    if (!employeeIdMapper->getRealId(abstractId)) {
      std::cerr << "Employee with abstract ID " << abstractId
                << " does not exist.\n";
      return;
    }

    int realEmployeeId = employeeIdMapper->getRealId(abstractId);
    std::string newFullName, newAddress, newDateOfBirth, newPosition;
    double newSalary;

    std::cout
        << "Enter new full name (or press Enter to keep the current value): ";
    std::cin.ignore();
    std::getline(std::cin, newFullName);

    std::cout
        << "Enter new address (or press Enter to keep the current value): ";
    std::getline(std::cin, newAddress);

    std::cout << "Enter new date of birth (or press Enter to keep the current "
                 "value): ";
    std::cin >> newDateOfBirth;

    std::cout
        << "Enter new position (or press Enter to keep the current value): ";
    std::cin.ignore();
    std::getline(std::cin, newPosition);

    std::cout
        << "Enter new salary (or press Enter to keep the current value): ";
    std::cin >> newSalary;

    for (Employees& employee : employeeIdMapper->employeeVector) {
      if (employee.getId() == employeeIdMapper->getAbstractId(realEmployeeId)) {
        if (!newFullName.empty()) {
          employee.setFullName(newFullName);
        } else {
          newFullName = employee.getFullName();
        }

        if (!newAddress.empty()) {
          employee.setAddress(newAddress);
        } else {
          newAddress = employee.getAddress();
        }

        if (!newDateOfBirth.empty()) {
          employee.setDateOfBirth(newDateOfBirth);
        } else {
          newDateOfBirth = employee.getDateOfBirth();
        }

        if (!newPosition.empty()) {
          employee.setPosition(newPosition);
        } else {
          newPosition = employee.getPosition();
        }

        if (newSalary != 0.0) {
          employee.setSalary(newSalary);
        } else {
          newSalary = employee.getSalary();
        }
        break;
      }
    }

    if (employeeGateway.updateEmployee(realEmployeeId, newFullName, newAddress,
                                       newDateOfBirth, newPosition,
                                       newSalary)) {
      std::cout << "Employee with abstract ID " << abstractId
                << " updated successfully.\n";
    } else {
      std::cerr << "Failed to update employee with abstract ID " << abstractId
                << ".\n";
    }
  }

 private:
  EmployeeGateway& employeeGateway;
  EmployeeIdMapper* employeeIdMapper;
};

#endif RPBD_EMPLOYEEMENU_H