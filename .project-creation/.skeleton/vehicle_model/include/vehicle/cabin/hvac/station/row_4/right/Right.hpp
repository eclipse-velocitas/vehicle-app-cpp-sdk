#ifndef VEHICLE_CABIN_HVAC_STATION_ROW_4_RIGHT_RIGHT_H
#define VEHICLE_CABIN_HVAC_STATION_ROW_4_RIGHT_RIGHT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::hvac::station::row_4::right {
using ParentClass = velocitas::Model;

/** Right model. */
class Right : public ParentClass {
public:

    Right(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		AirDistribution("AirDistribution", this),
		FanSpeed("FanSpeed", this),
		Temperature("Temperature", this)
    {}

    /**
    * AirDistribution: actuator
    * Direction of airstream
    *
    * Allowed values: UP, MIDDLE, DOWN
    **/
    velocitas::DataPointString AirDistribution;

    /**
    * FanSpeed: actuator
    * Fan Speed, 0 = off. 100 = max
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 FanSpeed;

    /**
    * Temperature: actuator
    * Temperature
    *
    * Unit: celsius
    **/
    velocitas::DataPointInt32 Temperature;

};

} // namespace vehicle::cabin::hvac::station::row_4::right

#endif // VEHICLE_CABIN_HVAC_STATION_ROW_4_RIGHT_RIGHT_H
