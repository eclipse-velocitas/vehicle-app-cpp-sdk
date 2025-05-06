#ifndef VEHICLE_CABIN_LIGHTS_SPOTLIGHT_ROW_2_ROW2_H
#define VEHICLE_CABIN_LIGHTS_SPOTLIGHT_ROW_2_ROW2_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::lights::spotlight::row_2 {
using ParentClass = velocitas::Model;

/** Row2 model. */
class Row2 : public ParentClass {
public:

    Row2(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsLeftOn("IsLeftOn", this),
		IsRightOn("IsRightOn", this),
		IsSharedOn("IsSharedOn", this)
    {}

    /**
    * IsLeftOn: actuator
    * Is light on the left side switched on
    *
    **/
    velocitas::DataPointBoolean IsLeftOn;

    /**
    * IsRightOn: actuator
    * Is light on the right side switched on
    *
    **/
    velocitas::DataPointBoolean IsRightOn;

    /**
    * IsSharedOn: sensor
    * Is a shared light across a specific row on
    *
    **/
    velocitas::DataPointBoolean IsSharedOn;

};

} // namespace vehicle::cabin::lights::spotlight::row_2

#endif // VEHICLE_CABIN_LIGHTS_SPOTLIGHT_ROW_2_ROW2_H
