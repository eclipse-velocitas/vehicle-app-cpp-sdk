#ifndef VEHICLE_CHASSIS_AXLE_ROW_1_WHEEL_RIGHT_RIGHT_H
#define VEHICLE_CHASSIS_AXLE_ROW_1_WHEEL_RIGHT_RIGHT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/chassis/axle/row_1/wheel/right/brake/Brake.hpp"
#include "vehicle/chassis/axle/row_1/wheel/right/tire/Tire.hpp"

#include <string>

namespace vehicle::chassis::axle::row_1::wheel::right {
using ParentClass = velocitas::Model;

/** Right model. */
class Right : public ParentClass {
public:

    Right(const std::string& name, ParentClass* parent) :
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
    vehicle::chassis::axle::row_1::wheel::right::brake::Brake Brake;

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
    vehicle::chassis::axle::row_1::wheel::right::tire::Tire Tire;

};

} // namespace vehicle::chassis::axle::row_1::wheel::right

#endif // VEHICLE_CHASSIS_AXLE_ROW_1_WHEEL_RIGHT_RIGHT_H
