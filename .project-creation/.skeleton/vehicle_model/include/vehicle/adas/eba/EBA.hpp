#ifndef VEHICLE_ADAS_EBA_EBA_H
#define VEHICLE_ADAS_EBA_EBA_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::adas::eba {
using ParentClass = velocitas::Model;

/** EBA model. */
class EBA : public ParentClass {
public:

    EBA(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsEnabled("IsEnabled", this),
		IsEngaged("IsEngaged", this),
		IsError("IsError", this)
    {}

    /**
    * IsEnabled: actuator
    * Indicates if EBA is enabled. True = Enabled. False = Disabled.
    *
    **/
    velocitas::DataPointBoolean IsEnabled;

    /**
    * IsEngaged: sensor
    * Indicates if EBA is currently regulating brake pressure. True = Engaged. False = Not Engaged.
    *
    **/
    velocitas::DataPointBoolean IsEngaged;

    /**
    * IsError: sensor
    * Indicates if EBA incurred an error condition. True = Error. False = No Error.
    *
    **/
    velocitas::DataPointBoolean IsError;

};

} // namespace vehicle::adas::eba

#endif // VEHICLE_ADAS_EBA_EBA_H
