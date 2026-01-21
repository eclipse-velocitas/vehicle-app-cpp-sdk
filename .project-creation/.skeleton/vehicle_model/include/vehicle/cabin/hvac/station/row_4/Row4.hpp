#ifndef VEHICLE_CABIN_HVAC_STATION_ROW_4_ROW4_H
#define VEHICLE_CABIN_HVAC_STATION_ROW_4_ROW4_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/hvac/station/row_4/left/Left.hpp"
#include "vehicle/cabin/hvac/station/row_4/right/Right.hpp"

#include <string>

namespace vehicle::cabin::hvac::station::row_4 {
using ParentClass = velocitas::Model;

/** Row4 model. */
class Row4 : public ParentClass {
public:

    Row4(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Left("Left", this),
		Right("Right", this)
    {}

    /**
    * Left: branch
    * HVAC for single station in the vehicle
    *
    **/
    vehicle::cabin::hvac::station::row_4::left::Left Left;

    /**
    * Right: branch
    * HVAC for single station in the vehicle
    *
    **/
    vehicle::cabin::hvac::station::row_4::right::Right Right;

};

} // namespace vehicle::cabin::hvac::station::row_4

#endif // VEHICLE_CABIN_HVAC_STATION_ROW_4_ROW4_H
