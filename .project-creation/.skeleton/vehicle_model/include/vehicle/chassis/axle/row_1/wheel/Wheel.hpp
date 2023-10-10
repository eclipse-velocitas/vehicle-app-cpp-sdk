#ifndef VEHICLE_CHASSIS_AXLE_ROW_1_WHEEL_WHEEL_H
#define VEHICLE_CHASSIS_AXLE_ROW_1_WHEEL_WHEEL_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/chassis/axle/row_1/wheel/left/Left.hpp"
#include "vehicle/chassis/axle/row_1/wheel/right/Right.hpp"

#include <string>

namespace vehicle::chassis::axle::row_1::wheel {
using ParentClass = velocitas::Model;

/** Wheel model. */
class Wheel : public ParentClass {
public:

    Wheel(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Left("Left", this),
		Right("Right", this)
    {}

    /**
    * Left: branch
    * Wheel signals for axle
    *
    **/
    vehicle::chassis::axle::row_1::wheel::left::Left Left;

    /**
    * Right: branch
    * Wheel signals for axle
    *
    **/
    vehicle::chassis::axle::row_1::wheel::right::Right Right;

};

} // namespace vehicle::chassis::axle::row_1::wheel

#endif // VEHICLE_CHASSIS_AXLE_ROW_1_WHEEL_WHEEL_H
