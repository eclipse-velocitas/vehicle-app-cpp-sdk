#ifndef VEHICLE_CHASSIS_STEERING_WHEEL_STEERINGWHEEL_H
#define VEHICLE_CHASSIS_STEERING_WHEEL_STEERINGWHEEL_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::chassis::steering_wheel {
using ParentClass = velocitas::Model;

/** SteeringWheel model. */
class SteeringWheel : public ParentClass {
public:

    SteeringWheel(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Angle("Angle", this),
		Extension("Extension", this),
		Position("Position", this),
		Tilt("Tilt", this)
    {}

    /**
    * Angle: sensor
    * Steering wheel angle. Positive = degrees to the left. Negative = degrees to the right.
    *
    * Unit: degrees
    **/
    velocitas::DataPointInt32 Angle;

    /**
    * Extension: actuator
    * Steering wheel column extension from dashboard. 0 = Closest to dashboard. 100 = Furthest from dashboard.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 Extension;

    /**
    * Position: attribute (string)
    * Position of the steering wheel on the left or right side of the vehicle.
    *
    * Allowed values: FRONT_LEFT, FRONT_RIGHT
    **/
    velocitas::DataPointString Position;

    /**
    * Tilt: actuator
    * Steering wheel column tilt. 0 = Lowest position. 100 = Highest position.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 Tilt;

};

} // namespace vehicle::chassis::steering_wheel

#endif // VEHICLE_CHASSIS_STEERING_WHEEL_STEERINGWHEEL_H
