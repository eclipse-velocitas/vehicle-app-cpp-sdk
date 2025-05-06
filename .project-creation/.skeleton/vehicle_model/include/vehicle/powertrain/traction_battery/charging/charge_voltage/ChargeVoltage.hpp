#ifndef VEHICLE_POWERTRAIN_TRACTION_BATTERY_CHARGING_CHARGE_VOLTAGE_CHARGEVOLTAGE_H
#define VEHICLE_POWERTRAIN_TRACTION_BATTERY_CHARGING_CHARGE_VOLTAGE_CHARGEVOLTAGE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::powertrain::traction_battery::charging::charge_voltage {
using ParentClass = velocitas::Model;

/** ChargeVoltage model. */
class ChargeVoltage : public ParentClass {
public:

    ChargeVoltage(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		DC("DC", this),
		Phase1("Phase1", this),
		Phase2("Phase2", this),
		Phase3("Phase3", this)
    {}

    /**
    * DC: sensor
    * Current DC charging voltage at charging inlet.
    *
    * Unit: V
    **/
    velocitas::DataPointFloat DC;

    /**
    * Phase1: sensor
    * Current AC charging voltage (rms) at inlet for Phase 1.
    *
    * Unit: V
    **/
    velocitas::DataPointFloat Phase1;

    /**
    * Phase2: sensor
    * Current AC charging voltage (rms) at inlet for Phase 2.
    *
    * Unit: V
    **/
    velocitas::DataPointFloat Phase2;

    /**
    * Phase3: sensor
    * Current AC charging voltage (rms) at inlet for Phase 3.
    *
    * Unit: V
    **/
    velocitas::DataPointFloat Phase3;

};

} // namespace vehicle::powertrain::traction_battery::charging::charge_voltage

#endif // VEHICLE_POWERTRAIN_TRACTION_BATTERY_CHARGING_CHARGE_VOLTAGE_CHARGEVOLTAGE_H
