#ifndef HOTELS_H
#define HOTELS_H

#include <string>

class Hotels {
 public:
  Hotels(const std::string& name, int hotelClass,
         const std::string& roomCategory)
      : name(name), hotelClass(hotelClass), roomCategory(roomCategory) {}

  const std::string& getName() const { return name; }
  int getHotelClass() const { return hotelClass; }
  const std::string& getRoomCategory() const { return roomCategory; }

  void setName(const std::string& name) { this->name = name; }
  void setHotelClass(int hotelClass) { this->hotelClass = hotelClass; }
  void setRoomCategory(const std::string& roomCategory) {
    this->roomCategory = roomCategory;
  }

 private:
  std::string name;
  int hotelClass;
  std::string roomCategory;
};

#endif
