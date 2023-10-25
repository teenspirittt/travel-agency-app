#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>

class Aircraft {
 public:
  Aircraft(int id, const std::string& aircraftType, int carrierId,
           const std::string& manufacturer, int capacity)
      : id(id),
        aircraftType(aircraftType),
        carrierId(carrierId),
        manufacturer(manufacturer),
        capacity(capacity) {}

  Aircraft(int id) : id(id) {}

  int getId() const { return id; }
  std::string getAircraftType() const { return aircraftType; }
  int getCarrierId() const { return carrierId; }
  std::string getManufacturer() const { return manufacturer; }
  int getCapacity() const { return capacity; }

  void setId(int id) { this->id = id; }
  void setAircraftType(const std::string& aircraftType) {
    this->aircraftType = aircraftType;
  }
  void setCarrierId(int carrierId) { this->carrierId = carrierId; }
  void setManufacturer(const std::string& manufacturer) {
    this->manufacturer = manufacturer;
  }
  void setCapacity(int capacity) { this->capacity = capacity; }

 private:
  int id;
  std::string aircraftType;
  int carrierId;
  std::string manufacturer;
  int capacity;
};

#endif
