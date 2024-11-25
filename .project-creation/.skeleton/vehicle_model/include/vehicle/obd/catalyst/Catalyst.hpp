#ifndef VEHICLE_OBD_CATALYST_CATALYST_H
#define VEHICLE_OBD_CATALYST_CATALYST_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/obd/catalyst/bank_1/Bank1.hpp"
#include "vehicle/obd/catalyst/bank_2/Bank2.hpp"

#include <string>

namespace vehicle::obd::catalyst {
using ParentClass = velocitas::Model;

/** Catalyst model. */
class Catalyst : public ParentClass {
public:

    Catalyst(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Bank1("Bank1", this),
		Bank2("Bank2", this)
    {}

    /**
    * Bank1: branch
    * Catalyst bank 1 signals
    *
    **/
    vehicle::obd::catalyst::bank_1::Bank1 Bank1;

    /**
    * Bank2: branch
    * Catalyst bank 2 signals
    *
    **/
    vehicle::obd::catalyst::bank_2::Bank2 Bank2;

};

} // namespace vehicle::obd::catalyst

#endif // VEHICLE_OBD_CATALYST_CATALYST_H
