#pragma once

#include "IdMapper.h"
#include "../../models/Carriers.h"

class CarrierIdMapper : public IdMapper {
public:
    static CarrierIdMapper &getInstance() {
        static CarrierIdMapper instance;
        return instance;
    }

    std::vector<Carrier> carrierVector;
private:
    CarrierIdMapper() = default;

};