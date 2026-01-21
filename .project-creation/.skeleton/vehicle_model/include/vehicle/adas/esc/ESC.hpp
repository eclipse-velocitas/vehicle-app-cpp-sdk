#ifndef VEHICLE_ADAS_ESC_ESC_H
#define VEHICLE_ADAS_ESC_ESC_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/adas/esc/road_friction/RoadFriction.hpp"

#include <string>

namespace vehicle::adas::esc {
using ParentClass = velocitas::Model;

/** ESC model. */
class ESC : public ParentClass {
public:

    ESC(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsEnabled("IsEnabled", this),
		IsEngaged("IsEngaged", this),
		IsError("IsError", this),
		IsStrongCrossWindDetected("IsStrongCrossWindDetected", this),
		RoadFriction("RoadFriction", this)
    {}

    /**
    * IsEnabled: actuator
    * Indicates if ESC is enabled. True = Enabled. False = Disabled.
    *
    **/
    velocitas::DataPointBoolean IsEnabled;

    /**
    * IsEngaged: sensor
    * Indicates if ESC is currently regulating vehicle stability. True = Engaged. False = Not Engaged.
    *
    **/
    velocitas::DataPointBoolean IsEngaged;

    /**
    * IsError: sensor
    * Indicates if ESC incurred an error condition. True = Error. False = No Error.
    *
    **/
    velocitas::DataPointBoolean IsError;

    /**
    * IsStrongCrossWindDetected: sensor
    * Indicates if the ESC system is detecting strong cross winds. True = Strong cross winds detected. False = No strong cross winds detected.
    *
    **/
    velocitas::DataPointBoolean IsStrongCrossWindDetected;

    /**
    * RoadFriction: branch
    * Road friction values reported by the ESC system.
    *
    **/
    vehicle::adas::esc::road_friction::RoadFriction RoadFriction;

};

} // namespace vehicle::adas::esc

#endif // VEHICLE_ADAS_ESC_ESC_H
