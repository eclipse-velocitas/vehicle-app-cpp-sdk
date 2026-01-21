#ifndef VEHICLE_CABIN_LIGHTS_SPOTLIGHT_ROW_1_ROW1_H
#define VEHICLE_CABIN_LIGHTS_SPOTLIGHT_ROW_1_ROW1_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::lights::spotlight::row_1 {
using ParentClass = velocitas::Model;

/** Row1 model. */
class Row1 : public ParentClass {
public:

    Row1(const std::string& name, ParentClass* parent) :
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

} // namespace vehicle::cabin::lights::spotlight::row_1

#endif // VEHICLE_CABIN_LIGHTS_SPOTLIGHT_ROW_1_ROW1_H
