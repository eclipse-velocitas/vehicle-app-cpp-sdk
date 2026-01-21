#ifndef VEHICLE_POWERTRAIN_TRACTION_BATTERY_DCDC_DCDC_H
#define VEHICLE_POWERTRAIN_TRACTION_BATTERY_DCDC_DCDC_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::powertrain::traction_battery::dcdc {
using ParentClass = velocitas::Model;

/** DCDC model. */
class DCDC : public ParentClass {
public:

    DCDC(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		PowerLoss("PowerLoss", this),
		Temperature("Temperature", this)
    {}

    /**
    * PowerLoss: sensor
    * Electrical energy lost by power dissipation to heat inside DC/DC converter.
    *
    * Unit: W
    **/
    velocitas::DataPointFloat PowerLoss;

    /**
    * Temperature: sensor
    * Current temperature of DC/DC converter converting battery high voltage to vehicle low voltage (typically 12 Volts).
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat Temperature;

};

} // namespace vehicle::powertrain::traction_battery::dcdc

#endif // VEHICLE_POWERTRAIN_TRACTION_BATTERY_DCDC_DCDC_H
