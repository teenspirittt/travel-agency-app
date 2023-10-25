#include <iostream>

#include "./gateways/AircraftGateway.h"
#include "./gateways/CarrierGateway.h"
#include "./gateways/ClientGateway.h"
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

    EmployeeTransfers employeeTransfers;

    // Проверка вставки записи о переводе
    if (employeeTransfers.insertTransfer(1, "Manager", "Promotion", "123456")) {
      std::cout << "Transfer inserted successfully." << std::endl;
    } else {
      std::cerr << "Failed to insert transfer." << std::endl;
    }

    // Проверка удаления записи о переводе
    if (employeeTransfers.deleteTransfer(3)) {
      std::cout << "Transfer deleted successfully." << std::endl;
    } else {
      std::cerr << "Failed to delete transfer." << std::endl;
    }

    // Проверка обновления записи о переводе
    if (employeeTransfers.updateTransfer(2, 2, "Supervisor", "New Assignment",
                                         "654321")) {
      std::cout << "Transfer updated successfully." << std::endl;
    } else {
      std::cerr << "Failed to update transfer." << std::endl;
    }

    // Проверка получения всех записей о переводах
    std::vector<int> allTransferIds;
    if (employeeTransfers.getAllTransfers(allTransferIds)) {
      std::cout << "All transfers retrieved successfully:" << std::endl;
      for (int id : allTransferIds) {
        std::cout << "Transfer ID: " << id << std::endl;
      }
    } else {
      std::cerr << "Failed to get all transfers." << std::endl;
    }

    int transferId = 6;  // Замените на существующий transferId
    int employeeId;
    std::string newPosition;
    std::string transferReason;
    std::string orderNumber;

    // Вызываем метод getTransfer
    if (employeeTransfers.getTransfer(transferId, employeeId, newPosition,
                                      transferReason, orderNumber)) {
      // Метод вернул true, что означает успешное получение данных о переводе
      std::cout << "good!  get transfer " << std::endl;
      // Теперь вы можете вывести или использовать полученные данные
      std::cout << "Transfer ID: " << transferId << std::endl;
      std::cout << "Employee ID: " << employeeId << std::endl;
      std::cout << "New Position: " << newPosition << std::endl;
      std::cout << "Transfer Reason: " << transferReason << std::endl;
      std::cout << "Order Number: " << orderNumber << std::endl;
    } else {
      // Метод вернул false, что означает, что данные о переводе не были найдены
      std::cerr << "Failed to get transfer with ID " << transferId << std::endl;
    }

    // Замените на реальный идентификатор.

    std::vector<int> matchingTransferIds;

    if (employeeTransfers.findTransfersByEmployee(1, matchingTransferIds)) {
      std::cout << "Transfers found for employee ID " << employeeId << ":"
                << std::endl;
      for (int transferId : matchingTransferIds) {
        std::cout << "Matching Transfer ID: " << transferId << std::endl;
      }
    } else {
      std::cerr << "No transfers found for employee ID " << employeeId
                << std::endl;
    }

    dbConnector.disconnect();
  } else {
    std::cerr << "Failed to connect to the database." << std::endl;
    return 0;
  }
}