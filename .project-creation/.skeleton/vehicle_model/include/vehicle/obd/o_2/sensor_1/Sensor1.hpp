#ifndef VEHICLE_OBD_O_2_SENSOR_1_SENSOR1_H
#define VEHICLE_OBD_O_2_SENSOR_1_SENSOR1_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::obd::o_2::sensor_1 {
using ParentClass = velocitas::Model;

/** Sensor1 model. */
class Sensor1 : public ParentClass {
public:

    Sensor1(const std::string& name, ParentClass* parent) :
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

} // namespace vehicle::obd::o_2::sensor_1

#endif // VEHICLE_OBD_O_2_SENSOR_1_SENSOR1_H
