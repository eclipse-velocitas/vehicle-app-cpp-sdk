#ifndef VEHICLE_POWERTRAIN_TRACTION_BATTERY_CHARGING_MAXIMUM_CHARGING_CURRENT_MAXIMUMCHARGINGCURRENT_H
#define VEHICLE_POWERTRAIN_TRACTION_BATTERY_CHARGING_MAXIMUM_CHARGING_CURRENT_MAXIMUMCHARGINGCURRENT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::powertrain::traction_battery::charging::maximum_charging_current {
using ParentClass = velocitas::Model;

/** MaximumChargingCurrent model. */
class MaximumChargingCurrent : public ParentClass {
public:

    MaximumChargingCurrent(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		DC("DC", this),
		Phase1("Phase1", this),
		Phase2("Phase2", this),
		Phase3("Phase3", this)
    {}

    /**
    * DC: sensor
    * Maximum DC charging current at inlet that can be accepted by the system.
    *
    * Unit: A
    **/
    velocitas::DataPointFloat DC;

    /**
    * Phase1: sensor
    * Maximum AC charging current (rms) at inlet for Phase 1 that can be accepted by the system.
    *
    * Unit: A
    **/
    velocitas::DataPointFloat Phase1;

    /**
    * Phase2: sensor
    * Maximum AC charging current (rms) at inlet for Phase 2 that can be accepted by the system.
    *
    * Unit: A
    **/
    velocitas::DataPointFloat Phase2;

    /**
    * Phase3: sensor
    * Maximum AC charging current (rms) at inlet for Phase 3 that can be accepted by the system.
    *
    * Unit: A
    **/
    velocitas::DataPointFloat Phase3;

};

} // namespace vehicle::powertrain::traction_battery::charging::maximum_charging_current

#endif // VEHICLE_POWERTRAIN_TRACTION_BATTERY_CHARGING_MAXIMUM_CHARGING_CURRENT_MAXIMUMCHARGINGCURRENT_H
