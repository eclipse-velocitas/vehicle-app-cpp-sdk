#ifndef VEHICLE_CHASSIS_AXLE_ROW_1_WHEEL_LEFT_TIRE_TIRE_H
#define VEHICLE_CHASSIS_AXLE_ROW_1_WHEEL_LEFT_TIRE_TIRE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::chassis::axle::row_1::wheel::left::tire {
using ParentClass = velocitas::Model;

/** Tire model. */
class Tire : public ParentClass {
public:

    Tire(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsPressureLow("IsPressureLow", this),
		Pressure("Pressure", this),
		Temperature("Temperature", this)
    {}

    /**
    * IsPressureLow: sensor
    * Tire Pressure Status. True = Low tire pressure. False = Good tire pressure.
    *
    **/
    velocitas::DataPointBoolean IsPressureLow;

    /**
    * Pressure: sensor
    * Tire pressure in kilo-Pascal.
    *
    * Unit: kPa
    **/
    velocitas::DataPointUint32 Pressure;

    /**
    * Temperature: sensor
    * Tire temperature in Celsius.
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat Temperature;

};

} // namespace vehicle::chassis::axle::row_1::wheel::left::tire

#endif // VEHICLE_CHASSIS_AXLE_ROW_1_WHEEL_LEFT_TIRE_TIRE_H
