#ifndef VEHICLE_ADAS_EBD_EBD_H
#define VEHICLE_ADAS_EBD_EBD_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::adas::ebd {
using ParentClass = velocitas::Model;

/** EBD model. */
class EBD : public ParentClass {
public:

    EBD(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsEnabled("IsEnabled", this),
		IsEngaged("IsEngaged", this),
		IsError("IsError", this)
    {}

    /**
    * IsEnabled: actuator
    * Indicates if EBD is enabled. True = Enabled. False = Disabled.
    *
    **/
    velocitas::DataPointBoolean IsEnabled;

    /**
    * IsEngaged: sensor
    * Indicates if EBD is currently regulating vehicle brakeforce distribution. True = Engaged. False = Not Engaged.
    *
    **/
    velocitas::DataPointBoolean IsEngaged;

    /**
    * IsError: sensor
    * Indicates if EBD incurred an error condition. True = Error. False = No Error.
    *
    **/
    velocitas::DataPointBoolean IsError;

};

} // namespace vehicle::adas::ebd

#endif // VEHICLE_ADAS_EBD_EBD_H
