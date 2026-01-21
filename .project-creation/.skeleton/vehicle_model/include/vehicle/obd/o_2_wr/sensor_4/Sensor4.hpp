#ifndef VEHICLE_OBD_O_2_WR_SENSOR_4_SENSOR4_H
#define VEHICLE_OBD_O_2_WR_SENSOR_4_SENSOR4_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::obd::o_2_wr::sensor_4 {
using ParentClass = velocitas::Model;

/** Sensor4 model. */
class Sensor4 : public ParentClass {
public:

    Sensor4(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Current("Current", this),
		Lambda("Lambda", this),
		Voltage("Voltage", this)
    {}

    /**
    * Current: sensor
    * PID 3x (byte CD) - Current for wide range/band oxygen sensor
    *
    * Unit: A
    **/
    velocitas::DataPointFloat Current;

    /**
    * Lambda: sensor
    * PID 2x (byte AB) and PID 3x (byte AB) - Lambda for wide range/band oxygen sensor
    *
    **/
    velocitas::DataPointFloat Lambda;

    /**
    * Voltage: sensor
    * PID 2x (byte CD) - Voltage for wide range/band oxygen sensor
    *
    * Unit: V
    **/
    velocitas::DataPointFloat Voltage;

};

} // namespace vehicle::obd::o_2_wr::sensor_4

#endif // VEHICLE_OBD_O_2_WR_SENSOR_4_SENSOR4_H
