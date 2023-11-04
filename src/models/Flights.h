#ifndef FLIGHTS_H
#define FLIGHTS_H

#include <string>

class Flights {
public:
    Flights(int id, const std::string &flightNumber, const std::string &departureDate,
            int aircraftId, const std::string &flightClass, int availableSeats)
            : id(id),
              flightNumber(flightNumber),
              departureDate(departureDate),
              aircraftId(aircraftId),
              flightClass(flightClass),
              availableSeats(availableSeats) {}

    explicit Flights(int id) {}

    const std::string &getFlightNumber() const { return flightNumber; }

    const std::string &getDepartureDate() const { return departureDate; }

    int getAircraftId() const { return aircraftId; }

    const std::string &getFlightClass() const { return flightClass; }

    int getAvailableSeats() const { return availableSeats; }

    void setFlightNumber(const std::string &flightNumber) {
        this->flightNumber = flightNumber;
    }

    void setDepartureDate(const std::string &departureDate) {
        this->departureDate = departureDate;
    }

    void setAircraftId(int aircraftId) { this->aircraftId = aircraftId; }

    void setFlightClass(const std::string &flightClass) {
        this->flightClass = flightClass;
    }

    void setAvailableSeats(int availableSeats) {
        this->availableSeats = availableSeats;
    }

    int getId() const {
        return id;
    }

private:
    int id{};
    std::string flightNumber;
    std::string departureDate;
    int aircraftId{};
    std::string flightClass;
    int availableSeats{};
};

#endif
