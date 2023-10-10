#ifndef VEHICLE_CHASSIS_AXLE_ROW_2_WHEEL_LEFT_LEFT_H
#define VEHICLE_CHASSIS_AXLE_ROW_2_WHEEL_LEFT_LEFT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/chassis/axle/row_2/wheel/left/brake/Brake.hpp"
#include "vehicle/chassis/axle/row_2/wheel/left/tire/Tire.hpp"

#include <string>

namespace vehicle::chassis::axle::row_2::wheel::left {
using ParentClass = velocitas::Model;

/** Left model. */
class Left : public ParentClass {
public:

    Left(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Brake("Brake", this),
		Speed("Speed", this),
		Tire("Tire", this)
    {}

    /**
    * Brake: branch
    * Brake signals for wheel
    *
    **/
    vehicle::chassis::axle::row_2::wheel::left::brake::Brake Brake;

    /**
    * Speed: sensor
    * Rotational speed of a vehicle's wheel.
    *
    * Unit: km/h
    **/
    velocitas::DataPointFloat Speed;

    /**
    * Tire: branch
    * Tire signals for wheel.
    *
    **/
    vehicle::chassis::axle::row_2::wheel::left::tire::Tire Tire;

};

} // namespace vehicle::chassis::axle::row_2::wheel::left

#endif // VEHICLE_CHASSIS_AXLE_ROW_2_WHEEL_LEFT_LEFT_H
