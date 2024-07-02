#ifndef VEHICLE_CABIN_SUNROOF_SUNROOF_H
#define VEHICLE_CABIN_SUNROOF_SUNROOF_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/sunroof/shade/Shade.hpp"

#include <string>

namespace vehicle::cabin::sunroof {
using ParentClass = velocitas::Model;

/** Sunroof model. */
class Sunroof : public ParentClass {
public:

    Sunroof(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Position("Position", this),
		Shade("Shade", this),
		Switch("Switch", this)
    {}

    /**
    * Position: sensor
    * Sunroof position. 0 = Fully closed 100 = Fully opened. -100 = Fully tilted.
    *
    * Value range: [-100, 100]
    **/
    velocitas::DataPointInt32 Position;

    /**
    * Shade: branch
    * Sun roof shade status.
    *
    **/
    vehicle::cabin::sunroof::shade::Shade Shade;

    /**
    * Switch: actuator
    * Switch controlling sliding action such as window, sunroof, or shade.
    *
    * Allowed values: INACTIVE, CLOSE, OPEN, ONE_SHOT_CLOSE, ONE_SHOT_OPEN, TILT_UP, TILT_DOWN
    **/
    velocitas::DataPointString Switch;

};

} // namespace vehicle::cabin::sunroof

#endif // VEHICLE_CABIN_SUNROOF_SUNROOF_H
