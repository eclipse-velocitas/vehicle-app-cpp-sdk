#ifndef VEHICLE_OBD_O_2_SENSOR_7_SENSOR7_H
#define VEHICLE_OBD_O_2_SENSOR_7_SENSOR7_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::obd::o_2::sensor_7 {
using ParentClass = velocitas::Model;

/** Sensor7 model. */
class Sensor7 : public ParentClass {
public:

    Sensor7(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		ShortTermFuelTrim("ShortTermFuelTrim", this),
		Voltage("Voltage", this)
    {}

    /**
    * ShortTermFuelTrim: sensor
    * PID 1x (byte B) - Short term fuel trim
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat ShortTermFuelTrim;

    /**
    * Voltage: sensor
    * PID 1x (byte A) - Sensor voltage
    *
    * Unit: V
    **/
    velocitas::DataPointFloat Voltage;

};

} // namespace vehicle::obd::o_2::sensor_7

#endif // VEHICLE_OBD_O_2_SENSOR_7_SENSOR7_H
