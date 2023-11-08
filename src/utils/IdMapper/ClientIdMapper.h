#pragma once
#include <vector>

#include "../../models/Clients.h"
#include "IdMapper.h"

class ClientIdMapper : public IdMapper {
 public:
  static ClientIdMapper& getInstance() {
    static ClientIdMapper instance;
    return instance;
  }

  std::vector<Clients> clientVector;

 private:
  ClientIdMapper() = default;
};