#ifndef VEHICLE_CHASSIS_CHASSIS_H
#define VEHICLE_CHASSIS_CHASSIS_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/chassis/accelerator/Accelerator.hpp"
#include "vehicle/chassis/axle/Axle.hpp"
#include "vehicle/chassis/brake/Brake.hpp"
#include "vehicle/chassis/parking_brake/ParkingBrake.hpp"
#include "vehicle/chassis/steering_wheel/SteeringWheel.hpp"

#include <string>

namespace vehicle::chassis {
using ParentClass = velocitas::Model;

/** Chassis model. */
class Chassis : public ParentClass {
public:

    Chassis(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Accelerator("Accelerator", this),
		Axle("Axle", this),
		AxleCount("AxleCount", this),
		Brake("Brake", this),
		ParkingBrake("ParkingBrake", this),
		SteeringWheel("SteeringWheel", this),
		Track("Track", this),
		Wheelbase("Wheelbase", this)
    {}

    /**
    * Accelerator: branch
    * Accelerator signals
    *
    **/
    vehicle::chassis::accelerator::Accelerator Accelerator;

    /**
    * Axle: branch
    * Axle signals
    *
    **/
    vehicle::chassis::axle::Axle Axle;

    /**
    * AxleCount: attribute (uint8)
    * Number of axles on the vehicle
    *
    **/
    velocitas::DataPointUint32 AxleCount;

    /**
    * Brake: branch
    * Brake system signals
    *
    **/
    vehicle::chassis::brake::Brake Brake;

    /**
    * ParkingBrake: branch
    * Parking brake signals
    *
    **/
    vehicle::chassis::parking_brake::ParkingBrake ParkingBrake;

    /**
    * SteeringWheel: branch
    * Steering wheel signals
    *
    **/
    vehicle::chassis::steering_wheel::SteeringWheel SteeringWheel;

    /**
    * Track: attribute (uint16)
    * Overall wheel tracking, in mm.
    *
    * Unit: mm
    **/
    velocitas::DataPointUint32 Track;

    /**
    * Wheelbase: attribute (uint16)
    * Overall wheel base, in mm.
    *
    * Unit: mm
    **/
    velocitas::DataPointUint32 Wheelbase;

};

} // namespace vehicle::chassis

#endif // VEHICLE_CHASSIS_CHASSIS_H
