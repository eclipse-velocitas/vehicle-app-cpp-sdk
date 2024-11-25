#ifndef VEHICLE_CABIN_REAR_SHADE_REARSHADE_H
#define VEHICLE_CABIN_REAR_SHADE_REARSHADE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::rear_shade {
using ParentClass = velocitas::Model;

/** RearShade model. */
class RearShade : public ParentClass {
public:

    RearShade(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Position("Position", this),
		Switch("Switch", this)
    {}

    /**
    * Position: actuator
    * Position of window blind. 0 = Fully retracted. 100 = Fully deployed.
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

} // namespace vehicle::cabin::rear_shade

#endif // VEHICLE_CABIN_REAR_SHADE_REARSHADE_H
