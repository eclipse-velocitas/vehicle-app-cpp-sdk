#ifndef VEHICLE_CABIN_LIGHTS_SPOTLIGHT_ROW_3_ROW3_H
#define VEHICLE_CABIN_LIGHTS_SPOTLIGHT_ROW_3_ROW3_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::lights::spotlight::row_3 {
using ParentClass = velocitas::Model;

/** Row3 model. */
class Row3 : public ParentClass {
public:

    Row3(const std::string& name, ParentClass* parent) :
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

} // namespace vehicle::cabin::lights::spotlight::row_3

#endif // VEHICLE_CABIN_LIGHTS_SPOTLIGHT_ROW_3_ROW3_H
