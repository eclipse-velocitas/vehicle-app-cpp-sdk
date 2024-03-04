#ifndef VEHICLE_CHASSIS_PARKING_BRAKE_PARKINGBRAKE_H
#define VEHICLE_CHASSIS_PARKING_BRAKE_PARKINGBRAKE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::chassis::parking_brake {
using ParentClass = velocitas::Model;

/** ParkingBrake model. */
class ParkingBrake : public ParentClass {
public:

    ParkingBrake(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsEngaged("IsEngaged", this)
    {}

    /**
    * IsEngaged: actuator
    * Parking brake status. True = Parking Brake is Engaged. False = Parking Brake is not Engaged.
    *
    **/
    velocitas::DataPointBoolean IsEngaged;

};

} // namespace vehicle::chassis::parking_brake

#endif // VEHICLE_CHASSIS_PARKING_BRAKE_PARKINGBRAKE_H
