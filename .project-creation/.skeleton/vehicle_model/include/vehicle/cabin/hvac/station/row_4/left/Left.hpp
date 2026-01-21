#ifndef VEHICLE_CABIN_HVAC_STATION_ROW_4_LEFT_LEFT_H
#define VEHICLE_CABIN_HVAC_STATION_ROW_4_LEFT_LEFT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::hvac::station::row_4::left {
using ParentClass = velocitas::Model;

/** Left model. */
class Left : public ParentClass {
public:

    Left(const std::string& name, ParentClass* parent) :
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

} // namespace vehicle::cabin::hvac::station::row_4::left

#endif // VEHICLE_CABIN_HVAC_STATION_ROW_4_LEFT_LEFT_H
