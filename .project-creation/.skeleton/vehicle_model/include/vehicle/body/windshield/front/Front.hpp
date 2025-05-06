#ifndef VEHICLE_BODY_WINDSHIELD_FRONT_FRONT_H
#define VEHICLE_BODY_WINDSHIELD_FRONT_FRONT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/body/windshield/front/washer_fluid/WasherFluid.hpp"
#include "vehicle/body/windshield/front/wiping/Wiping.hpp"

#include <string>

namespace vehicle::body::windshield::front {
using ParentClass = velocitas::Model;

/** Front model. */
class Front : public ParentClass {
public:

    Front(const std::string& name, ParentClass* parent) :
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
    vehicle::body::windshield::front::washer_fluid::WasherFluid WasherFluid;

    /**
    * Wiping: branch
    * Windshield wiper signals.
    *
    **/
    vehicle::body::windshield::front::wiping::Wiping Wiping;

};

} // namespace vehicle::body::windshield::front

#endif // VEHICLE_BODY_WINDSHIELD_FRONT_FRONT_H
