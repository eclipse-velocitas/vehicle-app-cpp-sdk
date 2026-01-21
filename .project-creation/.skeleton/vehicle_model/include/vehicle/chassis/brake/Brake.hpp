#ifndef VEHICLE_CHASSIS_BRAKE_BRAKE_H
#define VEHICLE_CHASSIS_BRAKE_BRAKE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::chassis::brake {
using ParentClass = velocitas::Model;

/** Brake model. */
class Brake : public ParentClass {
public:

    Brake(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsDriverEmergencyBrakingDetected("IsDriverEmergencyBrakingDetected", this),
		PedalPosition("PedalPosition", this)
    {}

    /**
    * IsDriverEmergencyBrakingDetected: sensor
    * Indicates if emergency braking initiated by driver is detected. True = Emergency braking detected. False = Emergency braking not detected.
    *
    * Detection of emergency braking can trigger Emergency Brake Assist (EBA) to engage.
    *
    **/
    velocitas::DataPointBoolean IsDriverEmergencyBrakingDetected;

    /**
    * PedalPosition: sensor
    * Brake pedal position as percent. 0 = Not depressed. 100 = Fully depressed.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 PedalPosition;

};

} // namespace vehicle::chassis::brake

#endif // VEHICLE_CHASSIS_BRAKE_BRAKE_H
