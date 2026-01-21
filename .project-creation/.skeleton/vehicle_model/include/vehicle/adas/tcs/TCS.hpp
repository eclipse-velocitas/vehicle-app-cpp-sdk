#ifndef VEHICLE_ADAS_TCS_TCS_H
#define VEHICLE_ADAS_TCS_TCS_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::adas::tcs {
using ParentClass = velocitas::Model;

/** TCS model. */
class TCS : public ParentClass {
public:

    TCS(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsEnabled("IsEnabled", this),
		IsEngaged("IsEngaged", this),
		IsError("IsError", this)
    {}

    /**
    * IsEnabled: actuator
    * Indicates if TCS is enabled. True = Enabled. False = Disabled.
    *
    **/
    velocitas::DataPointBoolean IsEnabled;

    /**
    * IsEngaged: sensor
    * Indicates if TCS is currently regulating traction. True = Engaged. False = Not Engaged.
    *
    **/
    velocitas::DataPointBoolean IsEngaged;

    /**
    * IsError: sensor
    * Indicates if TCS incurred an error condition. True = Error. False = No Error.
    *
    **/
    velocitas::DataPointBoolean IsError;

};

} // namespace vehicle::adas::tcs

#endif // VEHICLE_ADAS_TCS_TCS_H
