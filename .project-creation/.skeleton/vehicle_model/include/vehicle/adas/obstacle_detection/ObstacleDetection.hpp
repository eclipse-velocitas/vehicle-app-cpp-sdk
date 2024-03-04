#ifndef VEHICLE_ADAS_OBSTACLE_DETECTION_OBSTACLEDETECTION_H
#define VEHICLE_ADAS_OBSTACLE_DETECTION_OBSTACLEDETECTION_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::adas::obstacle_detection {
using ParentClass = velocitas::Model;

/** ObstacleDetection model. */
class ObstacleDetection : public ParentClass {
public:

    ObstacleDetection(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsEnabled("IsEnabled", this),
		IsError("IsError", this),
		IsWarning("IsWarning", this)
    {}

    /**
    * IsEnabled: actuator
    * Indicates if obstacle sensor system is enabled (i.e. monitoring for obstacles). True = Enabled. False = Disabled.
    *
    **/
    velocitas::DataPointBoolean IsEnabled;

    /**
    * IsError: sensor
    * Indicates if obstacle sensor system incurred an error condition. True = Error. False = No Error.
    *
    **/
    velocitas::DataPointBoolean IsError;

    /**
    * IsWarning: sensor
    * Indicates if obstacle sensor system registered an obstacle.
    *
    **/
    velocitas::DataPointBoolean IsWarning;

};

} // namespace vehicle::adas::obstacle_detection

#endif // VEHICLE_ADAS_OBSTACLE_DETECTION_OBSTACLEDETECTION_H
