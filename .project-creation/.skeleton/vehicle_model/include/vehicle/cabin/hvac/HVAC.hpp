#ifndef VEHICLE_CABIN_HVAC_HVAC_H
#define VEHICLE_CABIN_HVAC_HVAC_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/hvac/station/Station.hpp"

#include <string>

namespace vehicle::cabin::hvac {
using ParentClass = velocitas::Model;

/** HVAC model. */
class HVAC : public ParentClass {
public:

    HVAC(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		AmbientAirTemperature("AmbientAirTemperature", this),
		IsAirConditioningActive("IsAirConditioningActive", this),
		IsFrontDefrosterActive("IsFrontDefrosterActive", this),
		IsRearDefrosterActive("IsRearDefrosterActive", this),
		IsRecirculationActive("IsRecirculationActive", this),
		Station("Station", this)
    {}

    /**
    * AmbientAirTemperature: sensor
    * Ambient air temperature inside the vehicle.
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat AmbientAirTemperature;

    /**
    * IsAirConditioningActive: actuator
    * Is Air conditioning active.
    *
    **/
    velocitas::DataPointBoolean IsAirConditioningActive;

    /**
    * IsFrontDefrosterActive: actuator
    * Is front defroster active.
    *
    **/
    velocitas::DataPointBoolean IsFrontDefrosterActive;

    /**
    * IsRearDefrosterActive: actuator
    * Is rear defroster active.
    *
    **/
    velocitas::DataPointBoolean IsRearDefrosterActive;

    /**
    * IsRecirculationActive: actuator
    * Is recirculation active.
    *
    **/
    velocitas::DataPointBoolean IsRecirculationActive;

    /**
    * Station: branch
    * HVAC for single station in the vehicle
    *
    **/
    vehicle::cabin::hvac::station::Station Station;

};

} // namespace vehicle::cabin::hvac

#endif // VEHICLE_CABIN_HVAC_HVAC_H
