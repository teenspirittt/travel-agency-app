#ifndef RPBD_HOTELIDMAPPER_H
#define RPBD_HOTELIDMAPPER_H

#include <vector>

#include "../models/Hotels.h"
#include "IdMapper.h"

class HotelIdMapper : public IdMapper {
 public:
  static HotelIdMapper &getInstance() {
    static HotelIdMapper instance;
    return instance;
  }

  std::vector<Hotels> hotelVector;

 private:
  HotelIdMapper() = default;
};

#endif  // RPBD_HOTELIDMAPPER_H
