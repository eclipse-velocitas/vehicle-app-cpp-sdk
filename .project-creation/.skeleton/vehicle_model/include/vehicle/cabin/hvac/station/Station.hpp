#ifndef VEHICLE_CABIN_HVAC_STATION_STATION_H
#define VEHICLE_CABIN_HVAC_STATION_STATION_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/hvac/station/row_1/Row1.hpp"
#include "vehicle/cabin/hvac/station/row_2/Row2.hpp"
#include "vehicle/cabin/hvac/station/row_3/Row3.hpp"
#include "vehicle/cabin/hvac/station/row_4/Row4.hpp"

#include <string>

namespace vehicle::cabin::hvac::station {
using ParentClass = velocitas::Model;

/** Station model. */
class Station : public ParentClass {
public:

    Station(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Row1("Row1", this),
		Row2("Row2", this),
		Row3("Row3", this),
		Row4("Row4", this)
    {}

    /**
    * Row1: branch
    * HVAC for single station in the vehicle
    *
    **/
    vehicle::cabin::hvac::station::row_1::Row1 Row1;

    /**
    * Row2: branch
    * HVAC for single station in the vehicle
    *
    **/
    vehicle::cabin::hvac::station::row_2::Row2 Row2;

    /**
    * Row3: branch
    * HVAC for single station in the vehicle
    *
    **/
    vehicle::cabin::hvac::station::row_3::Row3 Row3;

    /**
    * Row4: branch
    * HVAC for single station in the vehicle
    *
    **/
    vehicle::cabin::hvac::station::row_4::Row4 Row4;

};

} // namespace vehicle::cabin::hvac::station

#endif // VEHICLE_CABIN_HVAC_STATION_STATION_H
