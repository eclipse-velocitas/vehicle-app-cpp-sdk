#ifndef VEHICLE_ADAS_LANE_DEPARTURE_DETECTION_LANEDEPARTUREDETECTION_H
#define VEHICLE_ADAS_LANE_DEPARTURE_DETECTION_LANEDEPARTUREDETECTION_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::adas::lane_departure_detection {
using ParentClass = velocitas::Model;

/** LaneDepartureDetection model. */
class LaneDepartureDetection : public ParentClass {
public:

    LaneDepartureDetection(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsEnabled("IsEnabled", this),
		IsError("IsError", this),
		IsWarning("IsWarning", this)
    {}

    /**
    * IsEnabled: actuator
    * Indicates if lane departure detection system is enabled. True = Enabled. False = Disabled.
    *
    **/
    velocitas::DataPointBoolean IsEnabled;

    /**
    * IsError: sensor
    * Indicates if lane departure system incurred an error condition. True = Error. False = No Error.
    *
    **/
    velocitas::DataPointBoolean IsError;

    /**
    * IsWarning: sensor
    * Indicates if lane departure detection registered a lane departure.
    *
    **/
    velocitas::DataPointBoolean IsWarning;

};

} // namespace vehicle::adas::lane_departure_detection

#endif // VEHICLE_ADAS_LANE_DEPARTURE_DETECTION_LANEDEPARTUREDETECTION_H
