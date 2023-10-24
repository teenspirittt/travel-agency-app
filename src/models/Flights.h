#ifndef FLIGHTS_H
#define FLIGHTS_H

#include <string>

class Flights {
 public:
  Flights(const std::string& flightNumber, const std::string& departureDate,
          int aircraftId, const std::string& flightClass, int availableSeats,
          int carrierId)
      : flightNumber(flightNumber),
        departureDate(departureDate),
        aircraftId(aircraftId),
        flightClass(flightClass),
        availableSeats(availableSeats),
        carrierId(carrierId) {}

  const std::string& getFlightNumber() const { return flightNumber; }
  const std::string& getDepartureDate() const { return departureDate; }
  int getAircraftId() const { return aircraftId; }
  const std::string& getFlightClass() const { return flightClass; }
  int getAvailableSeats() const { return availableSeats; }
  int getCarrierId() const { return carrierId; }

  void setFlightNumber(const std::string& flightNumber) {
    this->flightNumber = flightNumber;
  }
  void setDepartureDate(const std::string& departureDate) {
    this->departureDate = departureDate;
  }
  void setAircraftId(int aircraftId) { this->aircraftId = aircraftId; }
  void setFlightClass(const std::string& flightClass) {
    this->flightClass = flightClass;
  }
  void setAvailableSeats(int availableSeats) {
    this->availableSeats = availableSeats;
  }
  void setCarrierId(int carrierId) { this->carrierId = carrierId; }

 private:
  std::string flightNumber;
  std::string departureDate;
  int aircraftId;
  std::string flightClass;
  int availableSeats;
  int carrierId;
};

#endif
