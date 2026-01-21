#ifndef VEHICLE_POWERTRAIN_TRACTION_BATTERY_CHARGING_CHARGE_CURRENT_CHARGECURRENT_H
#define VEHICLE_POWERTRAIN_TRACTION_BATTERY_CHARGING_CHARGE_CURRENT_CHARGECURRENT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::powertrain::traction_battery::charging::charge_current {
using ParentClass = velocitas::Model;

/** ChargeCurrent model. */
class ChargeCurrent : public ParentClass {
public:

    ChargeCurrent(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		DC("DC", this),
		Phase1("Phase1", this),
		Phase2("Phase2", this),
		Phase3("Phase3", this)
    {}

    /**
    * DC: sensor
    * Current DC charging current at inlet. Negative if returning energy to grid.
    *
    * Unit: A
    **/
    velocitas::DataPointFloat DC;

    /**
    * Phase1: sensor
    * Current AC charging current (rms) at inlet for Phase 1. Negative if returning energy to grid.
    *
    * Unit: A
    **/
    velocitas::DataPointFloat Phase1;

    /**
    * Phase2: sensor
    * Current AC charging current (rms) at inlet for Phase 2. Negative if returning energy to grid.
    *
    * Unit: A
    **/
    velocitas::DataPointFloat Phase2;

    /**
    * Phase3: sensor
    * Current AC charging current (rms) at inlet for Phase 3. Negative if returning energy to grid.
    *
    * Unit: A
    **/
    velocitas::DataPointFloat Phase3;

};

} // namespace vehicle::powertrain::traction_battery::charging::charge_current

#endif // VEHICLE_POWERTRAIN_TRACTION_BATTERY_CHARGING_CHARGE_CURRENT_CHARGECURRENT_H
