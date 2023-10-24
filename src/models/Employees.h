#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#include <string>

class Employees {
 public:
  Employees(int id, const std::string& fullName, const std::string& address,
            const std::string& dateOfBirth, const std::string& position,
            double salary)
      : id(id),
        fullName(fullName),
        address(address),
        dateOfBirth(dateOfBirth),
        position(position),
        salary(salary) {}

  int getId() const { return id; }
  std::string getFullName() const { return fullName; }
  std::string getAddress() const { return address; }
  std::string getDateOfBirth() const { return dateOfBirth; }
  std::string getPosition() const { return position; }
  double getSalary() const { return salary; }

  void setId(int id) { this->id = id; }
  void setFullName(const std::string& fullName) { this->fullName = fullName; }
  void setAddress(const std::string& address) { this->address = address; }
  void setDateOfBirth(const std::string& dateOfBirth) {
    this->dateOfBirth = dateOfBirth;
  }
  void setPosition(const std::string& position) { this->position = position; }
  void setSalary(double salary) { this->salary = salary; }

 private:
  int id;
  std::string fullName;
  std::string address;
  std::string dateOfBirth;
  std::string position;
  double salary;
};

#endif
