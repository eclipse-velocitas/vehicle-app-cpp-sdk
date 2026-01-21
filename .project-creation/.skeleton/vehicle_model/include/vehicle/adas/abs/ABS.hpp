#ifndef VEHICLE_ADAS_ABS_ABS_H
#define VEHICLE_ADAS_ABS_ABS_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::adas::abs {
using ParentClass = velocitas::Model;

/** ABS model. */
class ABS : public ParentClass {
public:

    ABS(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsEnabled("IsEnabled", this),
		IsEngaged("IsEngaged", this),
		IsError("IsError", this)
    {}

    /**
    * IsEnabled: actuator
    * Indicates if ABS is enabled. True = Enabled. False = Disabled.
    *
    **/
    velocitas::DataPointBoolean IsEnabled;

    /**
    * IsEngaged: sensor
    * Indicates if ABS is currently regulating brake pressure. True = Engaged. False = Not Engaged.
    *
    **/
    velocitas::DataPointBoolean IsEngaged;

    /**
    * IsError: sensor
    * Indicates if ABS incurred an error condition. True = Error. False = No Error.
    *
    **/
    velocitas::DataPointBoolean IsError;

};

} // namespace vehicle::adas::abs

#endif // VEHICLE_ADAS_ABS_ABS_H
