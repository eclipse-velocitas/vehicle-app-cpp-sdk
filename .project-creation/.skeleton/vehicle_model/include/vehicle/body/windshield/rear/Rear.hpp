#ifndef VEHICLE_BODY_WINDSHIELD_REAR_REAR_H
#define VEHICLE_BODY_WINDSHIELD_REAR_REAR_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/body/windshield/rear/washer_fluid/WasherFluid.hpp"
#include "vehicle/body/windshield/rear/wiping/Wiping.hpp"

#include <string>

namespace vehicle::body::windshield::rear {
using ParentClass = velocitas::Model;

/** Rear model. */
class Rear : public ParentClass {
public:

    Rear(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsHeatingOn("IsHeatingOn", this),
		WasherFluid("WasherFluid", this),
		Wiping("Wiping", this)
    {}

    /**
    * IsHeatingOn: actuator
    * Windshield heater status. False - off, True - on.
    *
    **/
    velocitas::DataPointBoolean IsHeatingOn;

    /**
    * WasherFluid: branch
    * Windshield washer fluid signals
    *
    **/
    vehicle::body::windshield::rear::washer_fluid::WasherFluid WasherFluid;

    /**
    * Wiping: branch
    * Windshield wiper signals.
    *
    **/
    vehicle::body::windshield::rear::wiping::Wiping Wiping;

};

} // namespace vehicle::body::windshield::rear

#endif // VEHICLE_BODY_WINDSHIELD_REAR_REAR_H
