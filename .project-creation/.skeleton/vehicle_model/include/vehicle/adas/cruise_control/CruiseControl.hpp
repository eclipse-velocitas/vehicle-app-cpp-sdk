#ifndef VEHICLE_ADAS_CRUISE_CONTROL_CRUISECONTROL_H
#define VEHICLE_ADAS_CRUISE_CONTROL_CRUISECONTROL_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::adas::cruise_control {
using ParentClass = velocitas::Model;

/** CruiseControl model. */
class CruiseControl : public ParentClass {
public:

    CruiseControl(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsActive("IsActive", this),
		IsEnabled("IsEnabled", this),
		IsError("IsError", this),
		SpeedSet("SpeedSet", this)
    {}

    /**
    * IsActive: actuator
    * Indicates if cruise control system is active (i.e. actively controls speed). True = Active. False = Inactive.
    *
    **/
    velocitas::DataPointBoolean IsActive;

    /**
    * IsEnabled: actuator
    * Indicates if cruise control system is enabled (e.g. ready to receive configurations and settings) True = Enabled. False = Disabled.
    *
    **/
    velocitas::DataPointBoolean IsEnabled;

    /**
    * IsError: sensor
    * Indicates if cruise control system incurred an error condition. True = Error. False = No Error.
    *
    **/
    velocitas::DataPointBoolean IsError;

    /**
    * SpeedSet: actuator
    * Set cruise control speed in kilometers per hour.
    *
    * Unit: km/h
    **/
    velocitas::DataPointFloat SpeedSet;

};

} // namespace vehicle::adas::cruise_control

#endif // VEHICLE_ADAS_CRUISE_CONTROL_CRUISECONTROL_H
