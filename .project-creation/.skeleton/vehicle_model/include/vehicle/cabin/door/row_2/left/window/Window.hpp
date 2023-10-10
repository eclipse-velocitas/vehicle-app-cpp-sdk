#ifndef VEHICLE_CABIN_DOOR_ROW_2_LEFT_WINDOW_WINDOW_H
#define VEHICLE_CABIN_DOOR_ROW_2_LEFT_WINDOW_WINDOW_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::door::row_2::left::window {
using ParentClass = velocitas::Model;

/** Window model. */
class Window : public ParentClass {
public:

    Window(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsChildLockEngaged("IsChildLockEngaged", this),
		IsOpen("IsOpen", this),
		Position("Position", this),
		Switch("Switch", this)
    {}

    /**
    * IsChildLockEngaged: sensor
    * Is window child lock engaged. True = Engaged. False = Disengaged.
    *
    **/
    velocitas::DataPointBoolean IsChildLockEngaged;

    /**
    * IsOpen: sensor
    * Is window open or closed?
    *
    **/
    velocitas::DataPointBoolean IsOpen;

    /**
    * Position: sensor
    * Window position. 0 = Fully closed 100 = Fully opened.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 Position;

    /**
    * Switch: actuator
    * Switch controlling sliding action such as window, sunroof, or blind.
    *
    * Allowed values: INACTIVE, CLOSE, OPEN, ONE_SHOT_CLOSE, ONE_SHOT_OPEN
    **/
    velocitas::DataPointString Switch;

};

} // namespace vehicle::cabin::door::row_2::left::window

#endif // VEHICLE_CABIN_DOOR_ROW_2_LEFT_WINDOW_WINDOW_H
