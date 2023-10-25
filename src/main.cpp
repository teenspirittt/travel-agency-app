#include <iostream>

#include "./gateways/AircraftGateway.h"
#include "./gateways/CarrierGateway.h"
#include "./gateways/ClientGateway.h"
#include "./gateways/EmployeeGateway.h"
#include "./gateways/EmployeeTransfers.h"
#include "./gateways/FlightsGateway.h"
#include "./gateways/HotelGateway.h"
#include "./models/Aircraft.h"
#include "./utils/DataBaseConnector.h"
#include "./utils/DataBaseInitializer.h"

int main() {
  DataBaseConnector& dbConnector = DataBaseConnector::getInstance();

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
      std::cout << "Init database error";
    }

    EmployeeGateway employeeGateway;

    // Вставляем нового сотрудника
    if (employeeGateway.insertEmployee("John Doe", "123 Main St", "1990-01-15",
                                       "Manager", 55000.0)) {
      std::cout << "Employee inserted successfully." << std::endl;
    } else {
      std::cerr << "Failed to insert employee." << std::endl;
    }

    // Удаление сотрудника по ID (замените 1 на действительный ID)
    if (employeeGateway.deleteEmployee(3)) {
      std::cout << "Employee deleted successfully." << std::endl;
    } else {
      std::cerr << "Failed to delete employee." << std::endl;
    }

    std::string fullName, address, dateOfBirth, position;
    double salary;

    // Получение данных о сотруднике (замените 2 на действительный ID)
    if (employeeGateway.getEmployee(2, fullName, address, dateOfBirth, position,
                                    salary)) {
      std::cout << "Employee get successfully." << std::endl;
      std::cout << "Full Name: " << fullName << std::endl;
      std::cout << "Address: " << address << std::endl;
      std::cout << "Date of Birth: " << dateOfBirth << std::endl;
      std::cout << "Position: " << position << std::endl;
      std::cout << "Salary: " << salary << std::endl;
    } else {
      std::cerr << "Failed to get employee." << std::endl;
    }

    // Обновление данных о сотруднике (замените 3 на действительный ID)
    if (employeeGateway.updateEmployee(13, "Alice Smith", "456 Elm St",
                                       "1988-06-20", "Supervisor", 60000.0)) {
      std::cout << "Employee updated successfully." << std::endl;
    } else {
      std::cerr << "Failed to update employee." << std::endl;
    }

    std::vector<int> matchingEmployeeIds;

    // Поиск сотрудников по должности
    if (employeeGateway.findEmployeesByPosition("Manager",
                                                matchingEmployeeIds)) {
      std::cout
          << "Employees with the position 'Manager' were found successfully."
          << std::endl;
      for (int employeeId : matchingEmployeeIds) {
        std::cout << "Matching Employee ID: " << employeeId << std::endl;
      }
    } else {
      std::cerr << "Failed to find employees by position." << std::endl;
    }

    dbConnector.disconnect();
  } else {
    std::cerr << "Failed to connect to the database." << std::endl;
    return 0;
  }
}