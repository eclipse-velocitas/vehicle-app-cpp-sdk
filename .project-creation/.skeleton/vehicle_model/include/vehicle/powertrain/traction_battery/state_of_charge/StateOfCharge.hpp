#ifndef VEHICLE_POWERTRAIN_TRACTION_BATTERY_STATE_OF_CHARGE_STATEOFCHARGE_H
#define VEHICLE_POWERTRAIN_TRACTION_BATTERY_STATE_OF_CHARGE_STATEOFCHARGE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::powertrain::traction_battery::state_of_charge {
using ParentClass = velocitas::Model;

/** StateOfCharge model. */
class StateOfCharge : public ParentClass {
public:

    StateOfCharge(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Current("Current", this),
		Displayed("Displayed", this)
    {}

    /**
    * Current: sensor
    * Physical state of charge of the high voltage battery, relative to net capacity. This is not necessarily the state of charge being displayed to the customer.
    *
    * Value range: [0, 100.0]
    * Unit: percent
    **/
    velocitas::DataPointFloat Current;

    /**
    * Displayed: sensor
    * State of charge displayed to the customer.
    *
    * Value range: [0, 100.0]
    * Unit: percent
    **/
    velocitas::DataPointFloat Displayed;

};

} // namespace vehicle::powertrain::traction_battery::state_of_charge

#endif // VEHICLE_POWERTRAIN_TRACTION_BATTERY_STATE_OF_CHARGE_STATEOFCHARGE_H
