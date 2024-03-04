#ifndef VEHICLE_CHASSIS_AXLE_AXLE_H
#define VEHICLE_CHASSIS_AXLE_AXLE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/chassis/axle/row_1/Row1.hpp"
#include "vehicle/chassis/axle/row_2/Row2.hpp"

#include <string>

namespace vehicle::chassis::axle {
using ParentClass = velocitas::Model;

/** Axle model. */
class Axle : public ParentClass {
public:

    Axle(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Row1("Row1", this),
		Row2("Row2", this)
    {}

    /**
    * Row1: branch
    * Axle signals
    *
    **/
    vehicle::chassis::axle::row_1::Row1 Row1;

    /**
    * Row2: branch
    * Axle signals
    *
    **/
    vehicle::chassis::axle::row_2::Row2 Row2;

};

} // namespace vehicle::chassis::axle

#endif // VEHICLE_CHASSIS_AXLE_AXLE_H
