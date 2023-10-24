#ifndef CARRIER_H
#define CARRIER_H

#include <string>

class Carrier {
 public:
  Carrier(int id, const std::string& name) : id(id), name(name) {}

  int getId() const { return id; }
  std::string getName() const { return name; }

  void setId(int id) { this->id = id; }
  void setName(const std::string& name) { this->name = name; }

 private:
  int id;
  std::string name;
};

#endif
