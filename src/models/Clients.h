#ifndef CLIENTS_H
#define CLIENTS_H

#include <string>

class Clients {
 public:
  Clients(int id, const std::string& fullName, const std::string& phone,
          const std::string& orderDate, int routeId,
          const std::string& classType, const std::string& seat)
      : id(id),
        fullName(fullName),
        phone(phone),
        orderDate(orderDate),
        routeId(routeId),
        classType(classType),
        seat(seat) {}

  int getId() const { return id; }
  std::string getFullName() const { return fullName; }
  std::string getPhone() const { return phone; }
  std::string getOrderDate() const { return orderDate; }
  int getRouteId() const { return routeId; }
  std::string getClassType() const { return classType; }
  std::string getSeat() const { return seat; }

  void setId(int id) { this->id = id; }
  void setFullName(const std::string& fullName) { this->fullName = fullName; }
  void setPhone(const std::string& phone) { this->phone = phone; }
  void setOrderDate(const std::string& orderDate) {
    this->orderDate = orderDate;
  }
  void setRouteId(int routeId) { this->routeId = routeId; }
  void setClassType(const std::string& classType) {
    this->classType = classType;
  }
  void setSeat(const std::string& seat) { this->seat = seat; }

 private:
  int id;
  std::string fullName;
  std::string phone;
  std::string orderDate;
  int routeId;
  std::string classType;
  std::string seat;
};

#endif
