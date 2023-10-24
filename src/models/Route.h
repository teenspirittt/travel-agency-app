#ifndef ROUTE_H
#define ROUTE_H

#include <string>

class Route {
 public:
  Route(const std::string& name, const std::string& country,
        const std::string& city, int duration, int hotelId, int flightId,
        int employeeId, const std::string& agencyRepName,
        const std::string& agencyRepPhone)
      : name(name),
        country(country),
        city(city),
        duration(duration),
        hotelId(hotelId),
        flightId(flightId),
        employeeId(employeeId),
        agencyRepName(agencyRepName),
        agencyRepPhone(agencyRepPhone) {}

  const std::string& getName() const { return name; }
  const std::string& getCountry() const { return country; }
  const std::string& getCity() const { return city; }
  int getDuration() const { return duration; }
  int getHotelId() const { return hotelId; }
  int getFlightId() const { return flightId; }
  int getEmployeeId() const { return employeeId; }
  const std::string& getAgencyRepName() const { return agencyRepName; }
  const std::string& getAgencyRepPhone() const { return agencyRepPhone; }

  void setName(const std::string& name) { this->name = name; }
  void setCountry(const std::string& country) { this->country = country; }
  void setCity(const std::string& city) { this->city = city; }
  void setDuration(int duration) { this->duration = duration; }
  void setHotelId(int hotelId) { this->hotelId = hotelId; }
  void setFlightId(int flightId) { this->flightId = flightId; }
  void setEmployeeId(int employeeId) { this->employeeId = employeeId; }
  void setAgencyRepName(const std::string& agencyRepName) {
    this->agencyRepName = agencyRepName;
  }
  void setAgencyRepPhone(const std::string& agencyRepPhone) {
    this->agencyRepPhone = agencyRepPhone;
  }

 private:
  std::string name;
  std::string country;
  std::string city;
  int duration;
  int hotelId;
  int flightId;
  int employeeId;
  std::string agencyRepName;
  std::string agencyRepPhone;
};

#endif
