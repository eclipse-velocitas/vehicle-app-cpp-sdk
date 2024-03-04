#ifndef VEHICLE_OBD_O_2_O2_H
#define VEHICLE_OBD_O_2_O2_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/obd/o_2/sensor_1/Sensor1.hpp"
#include "vehicle/obd/o_2/sensor_2/Sensor2.hpp"
#include "vehicle/obd/o_2/sensor_3/Sensor3.hpp"
#include "vehicle/obd/o_2/sensor_4/Sensor4.hpp"
#include "vehicle/obd/o_2/sensor_5/Sensor5.hpp"
#include "vehicle/obd/o_2/sensor_6/Sensor6.hpp"
#include "vehicle/obd/o_2/sensor_7/Sensor7.hpp"
#include "vehicle/obd/o_2/sensor_8/Sensor8.hpp"

#include <string>

namespace vehicle::obd::o_2 {
using ParentClass = velocitas::Model;

/** O2 model. */
class O2 : public ParentClass {
public:

    O2(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Sensor1("Sensor1", this),
		Sensor2("Sensor2", this),
		Sensor3("Sensor3", this),
		Sensor4("Sensor4", this),
		Sensor5("Sensor5", this),
		Sensor6("Sensor6", this),
		Sensor7("Sensor7", this),
		Sensor8("Sensor8", this)
    {}

    /**
    * Sensor1: branch
    * Oxygen sensors (PID 14 - PID 1B)
    *
    **/
    vehicle::obd::o_2::sensor_1::Sensor1 Sensor1;

    /**
    * Sensor2: branch
    * Oxygen sensors (PID 14 - PID 1B)
    *
    **/
    vehicle::obd::o_2::sensor_2::Sensor2 Sensor2;

    /**
    * Sensor3: branch
    * Oxygen sensors (PID 14 - PID 1B)
    *
    **/
    vehicle::obd::o_2::sensor_3::Sensor3 Sensor3;

    /**
    * Sensor4: branch
    * Oxygen sensors (PID 14 - PID 1B)
    *
    **/
    vehicle::obd::o_2::sensor_4::Sensor4 Sensor4;

    /**
    * Sensor5: branch
    * Oxygen sensors (PID 14 - PID 1B)
    *
    **/
    vehicle::obd::o_2::sensor_5::Sensor5 Sensor5;

    /**
    * Sensor6: branch
    * Oxygen sensors (PID 14 - PID 1B)
    *
    **/
    vehicle::obd::o_2::sensor_6::Sensor6 Sensor6;

    /**
    * Sensor7: branch
    * Oxygen sensors (PID 14 - PID 1B)
    *
    **/
    vehicle::obd::o_2::sensor_7::Sensor7 Sensor7;

    /**
    * Sensor8: branch
    * Oxygen sensors (PID 14 - PID 1B)
    *
    **/
    vehicle::obd::o_2::sensor_8::Sensor8 Sensor8;

};

} // namespace vehicle::obd::o_2

#endif // VEHICLE_OBD_O_2_O2_H
