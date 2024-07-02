#ifndef VEHICLE_BODY_LIGHTS_LIGHTS_H
#define VEHICLE_BODY_LIGHTS_LIGHTS_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::body::lights {
using ParentClass = velocitas::Model;

/** Lights model. */
class Lights : public ParentClass {
public:

    Lights(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsBackupOn("IsBackupOn", this),
		IsBrakeOn("IsBrakeOn", this),
		IsFrontFogOn("IsFrontFogOn", this),
		IsHazardOn("IsHazardOn", this),
		IsHighBeamOn("IsHighBeamOn", this),
		IsLeftIndicatorOn("IsLeftIndicatorOn", this),
		IsLowBeamOn("IsLowBeamOn", this),
		IsParkingOn("IsParkingOn", this),
		IsRearFogOn("IsRearFogOn", this),
		IsRightIndicatorOn("IsRightIndicatorOn", this),
		IsRunningOn("IsRunningOn", this)
    {}

    /**
    * IsBackupOn: actuator
    * Is backup (reverse) light on?
    *
    **/
    velocitas::DataPointBoolean IsBackupOn;

    /**
    * IsBrakeOn: actuator
    * Is brake light on?
    *
    **/
    velocitas::DataPointBoolean IsBrakeOn;

    /**
    * IsFrontFogOn: actuator
    * Is front fog light on?
    *
    **/
    velocitas::DataPointBoolean IsFrontFogOn;

    /**
    * IsHazardOn: actuator
    * Are hazards on?
    *
    **/
    velocitas::DataPointBoolean IsHazardOn;

    /**
    * IsHighBeamOn: actuator
    * Is high beam on?
    *
    **/
    velocitas::DataPointBoolean IsHighBeamOn;

    /**
    * IsLeftIndicatorOn: actuator
    * Is left indicator flashing?
    *
    **/
    velocitas::DataPointBoolean IsLeftIndicatorOn;

    /**
    * IsLowBeamOn: actuator
    * Is low beam on?
    *
    **/
    velocitas::DataPointBoolean IsLowBeamOn;

    /**
    * IsParkingOn: actuator
    * Is parking light on?
    *
    **/
    velocitas::DataPointBoolean IsParkingOn;

    /**
    * IsRearFogOn: actuator
    * Is rear fog light on?
    *
    **/
    velocitas::DataPointBoolean IsRearFogOn;

    /**
    * IsRightIndicatorOn: actuator
    * Is right indicator flashing?
    *
    **/
    velocitas::DataPointBoolean IsRightIndicatorOn;

    /**
    * IsRunningOn: actuator
    * Are running lights on?
    *
    **/
    velocitas::DataPointBoolean IsRunningOn;

};

} // namespace vehicle::body::lights

#endif // VEHICLE_BODY_LIGHTS_LIGHTS_H
