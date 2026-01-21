#ifndef VEHICLE_OBD_O_2_WR_O2WR_H
#define VEHICLE_OBD_O_2_WR_O2WR_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/obd/o_2_wr/sensor_1/Sensor1.hpp"
#include "vehicle/obd/o_2_wr/sensor_2/Sensor2.hpp"
#include "vehicle/obd/o_2_wr/sensor_3/Sensor3.hpp"
#include "vehicle/obd/o_2_wr/sensor_4/Sensor4.hpp"
#include "vehicle/obd/o_2_wr/sensor_5/Sensor5.hpp"
#include "vehicle/obd/o_2_wr/sensor_6/Sensor6.hpp"
#include "vehicle/obd/o_2_wr/sensor_7/Sensor7.hpp"
#include "vehicle/obd/o_2_wr/sensor_8/Sensor8.hpp"

#include <string>

namespace vehicle::obd::o_2_wr {
using ParentClass = velocitas::Model;

/** O2WR model. */
class O2WR : public ParentClass {
public:

    O2WR(const std::string& name, ParentClass* parent) :
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
    * Wide range/band oxygen sensors (PID 24 - 2B and PID 34 - 3B)
    *
    **/
    vehicle::obd::o_2_wr::sensor_1::Sensor1 Sensor1;

    /**
    * Sensor2: branch
    * Wide range/band oxygen sensors (PID 24 - 2B and PID 34 - 3B)
    *
    **/
    vehicle::obd::o_2_wr::sensor_2::Sensor2 Sensor2;

    /**
    * Sensor3: branch
    * Wide range/band oxygen sensors (PID 24 - 2B and PID 34 - 3B)
    *
    **/
    vehicle::obd::o_2_wr::sensor_3::Sensor3 Sensor3;

    /**
    * Sensor4: branch
    * Wide range/band oxygen sensors (PID 24 - 2B and PID 34 - 3B)
    *
    **/
    vehicle::obd::o_2_wr::sensor_4::Sensor4 Sensor4;

    /**
    * Sensor5: branch
    * Wide range/band oxygen sensors (PID 24 - 2B and PID 34 - 3B)
    *
    **/
    vehicle::obd::o_2_wr::sensor_5::Sensor5 Sensor5;

    /**
    * Sensor6: branch
    * Wide range/band oxygen sensors (PID 24 - 2B and PID 34 - 3B)
    *
    **/
    vehicle::obd::o_2_wr::sensor_6::Sensor6 Sensor6;

    /**
    * Sensor7: branch
    * Wide range/band oxygen sensors (PID 24 - 2B and PID 34 - 3B)
    *
    **/
    vehicle::obd::o_2_wr::sensor_7::Sensor7 Sensor7;

    /**
    * Sensor8: branch
    * Wide range/band oxygen sensors (PID 24 - 2B and PID 34 - 3B)
    *
    **/
    vehicle::obd::o_2_wr::sensor_8::Sensor8 Sensor8;

};

} // namespace vehicle::obd::o_2_wr

#endif // VEHICLE_OBD_O_2_WR_O2WR_H
