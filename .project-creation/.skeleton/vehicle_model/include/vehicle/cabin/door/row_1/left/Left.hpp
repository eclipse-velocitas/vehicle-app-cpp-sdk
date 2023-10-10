#ifndef VEHICLE_CABIN_DOOR_ROW_1_LEFT_LEFT_H
#define VEHICLE_CABIN_DOOR_ROW_1_LEFT_LEFT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/door/row_1/left/shade/Shade.hpp"
#include "vehicle/cabin/door/row_1/left/window/Window.hpp"

#include <string>

namespace vehicle::cabin::door::row_1::left {
using ParentClass = velocitas::Model;

/** Left model. */
class Left : public ParentClass {
public:

    Left(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsChildLockActive("IsChildLockActive", this),
		IsLocked("IsLocked", this),
		IsOpen("IsOpen", this),
		Shade("Shade", this),
		Window("Window", this)
    {}

    /**
    * IsChildLockActive: sensor
    * Is door child lock engaged. True = Engaged. False = Disengaged.
    *
    **/
    velocitas::DataPointBoolean IsChildLockActive;

    /**
    * IsLocked: actuator
    * Is door locked or unlocked. True = Locked. False = Unlocked.
    *
    **/
    velocitas::DataPointBoolean IsLocked;

    /**
    * IsOpen: actuator
    * Is door open or closed
    *
    **/
    velocitas::DataPointBoolean IsOpen;

    /**
    * Shade: branch
    * Side window shade
    *
    **/
    vehicle::cabin::door::row_1::left::shade::Shade Shade;

    /**
    * Window: branch
    * Door window status
    *
    **/
    vehicle::cabin::door::row_1::left::window::Window Window;

};

} // namespace vehicle::cabin::door::row_1::left

#endif // VEHICLE_CABIN_DOOR_ROW_1_LEFT_LEFT_H
