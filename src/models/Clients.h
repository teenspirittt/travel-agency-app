#ifndef CLIENTS_H
#define CLIENTS_H

#include <string>

class Clients {
 public:
  Clients(int id, const std::string& fullName, const std::string& phone,
          const std::string& orderDate, const std::string& seat)
      : id(id),
        fullName(fullName),
        phone(phone),
        orderDate(orderDate),
        seat(seat) {}

  int getId() const { return id; }
  std::string getFullName() const { return fullName; }
  std::string getPhone() const { return phone; }
  std::string getOrderDate() const { return orderDate; }
  std::string getSeat() const { return seat; }

  void setId(int id) { this->id = id; }
  void setFullName(const std::string& fullName) { this->fullName = fullName; }
  void setPhone(const std::string& phone) { this->phone = phone; }
  void setOrderDate(const std::string& orderDate) {
    this->orderDate = orderDate;
  }
  void setSeat(const std::string& seat) { this->seat = seat; }

 private:
  int id;
  std::string fullName;
  std::string phone;
  std::string orderDate;
  std::string seat;
};

#endif
