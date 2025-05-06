#ifndef VEHICLE_OBD_CATALYST_BANK_2_BANK2_H
#define VEHICLE_OBD_CATALYST_BANK_2_BANK2_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::obd::catalyst::bank_2 {
using ParentClass = velocitas::Model;

/** Bank2 model. */
class Bank2 : public ParentClass {
public:

    Bank2(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Temperature1("Temperature1", this),
		Temperature2("Temperature2", this)
    {}

    /**
    * Temperature1: sensor
    * PID 3D - Catalyst temperature from bank 2, sensor 1
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat Temperature1;

    /**
    * Temperature2: sensor
    * PID 3F - Catalyst temperature from bank 2, sensor 2
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat Temperature2;

};

} // namespace vehicle::obd::catalyst::bank_2

#endif // VEHICLE_OBD_CATALYST_BANK_2_BANK2_H
