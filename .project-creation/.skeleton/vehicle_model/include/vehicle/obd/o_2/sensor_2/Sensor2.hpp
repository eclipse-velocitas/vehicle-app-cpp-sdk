#ifndef VEHICLE_OBD_O_2_SENSOR_2_SENSOR2_H
#define VEHICLE_OBD_O_2_SENSOR_2_SENSOR2_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::obd::o_2::sensor_2 {
using ParentClass = velocitas::Model;

/** Sensor2 model. */
class Sensor2 : public ParentClass {
public:

    Sensor2(const std::string& name, ParentClass* parent) :
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

} // namespace vehicle::obd::o_2::sensor_2

#endif // VEHICLE_OBD_O_2_SENSOR_2_SENSOR2_H
