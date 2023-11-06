#ifndef EMPLOYEETRANSFERS_H
#define EMPLOYEETRANSFERS_H

#include <string>

class EmployeeTransfers {
 public:
  EmployeeTransfers(int id, int employeeId, const std::string& newPosition,
                    const std::string& transferReason,
                    const std::string& orderNumber,
                    const std::string& orderDate)
      : id(id),
        employeeId(employeeId),
        newPosition(newPosition),
        transferReason(transferReason),
        orderNumber(orderNumber),
        orderDate(orderDate) {}

  int getEmployeeId() const { return employeeId; }
  const std::string& getNewPosition() const { return newPosition; }
  const std::string& getTransferReason() const { return transferReason; }
  const std::string& getOrderNumber() const { return orderNumber; }
  const std::string& getOrderDate() const { return orderDate; }

  void setEmployeeId(int employeeId) { this->employeeId = employeeId; }
  void setNewPosition(const std::string& newPosition) {
    this->newPosition = newPosition;
  }
  void setTransferReason(const std::string& transferReason) {
    this->transferReason = transferReason;
  }
  void setOrderNumber(const std::string& orderNumber) {
    this->orderNumber = orderNumber;
  }
  void setOrderDate(const std::string& orderDate) {
    this->orderDate = orderDate;
  }

  int getId() const { return id; }

 private:
  int id;
  int employeeId;
  std::string newPosition;
  std::string transferReason;
  std::string orderNumber;
  std::string orderDate;
};

#endif  // EMPLOYEETRANSFERS_H
