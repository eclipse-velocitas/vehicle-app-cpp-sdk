#ifndef VEHICLE_ANGULAR_VELOCITY_ANGULARVELOCITY_H
#define VEHICLE_ANGULAR_VELOCITY_ANGULARVELOCITY_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::angular_velocity {
using ParentClass = velocitas::Model;

/** AngularVelocity model. */
class AngularVelocity : public ParentClass {
public:

    AngularVelocity(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Pitch("Pitch", this),
		Roll("Roll", this),
		Yaw("Yaw", this)
    {}

    /**
    * Pitch: sensor
    * Vehicle rotation rate along Y (lateral).
    *
    * Unit: degrees/s
    **/
    velocitas::DataPointFloat Pitch;

    /**
    * Roll: sensor
    * Vehicle rotation rate along X (longitudinal).
    *
    * Unit: degrees/s
    **/
    velocitas::DataPointFloat Roll;

    /**
    * Yaw: sensor
    * Vehicle rotation rate along Z (vertical).
    *
    * Unit: degrees/s
    **/
    velocitas::DataPointFloat Yaw;

};

} // namespace vehicle::angular_velocity

#endif // VEHICLE_ANGULAR_VELOCITY_ANGULARVELOCITY_H
