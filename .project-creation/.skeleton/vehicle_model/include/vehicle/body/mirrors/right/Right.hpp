#ifndef VEHICLE_BODY_MIRRORS_RIGHT_RIGHT_H
#define VEHICLE_BODY_MIRRORS_RIGHT_RIGHT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::body::mirrors::right {
using ParentClass = velocitas::Model;

/** Right model. */
class Right : public ParentClass {
public:

    Right(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsHeatingOn("IsHeatingOn", this),
		Pan("Pan", this),
		Tilt("Tilt", this)
    {}

    /**
    * IsHeatingOn: actuator
    * Mirror Heater on or off. True = Heater On. False = Heater Off.
    *
    **/
    velocitas::DataPointBoolean IsHeatingOn;

    /**
    * Pan: actuator
    * Mirror pan as a percent. 0 = Center Position. 100 = Fully Left Position. -100 = Fully Right Position.
    *
    * Value range: [-100, 100]
    * Unit: percent
    **/
    velocitas::DataPointInt32 Pan;

    /**
    * Tilt: actuator
    * Mirror tilt as a percent. 0 = Center Position. 100 = Fully Upward Position. -100 = Fully Downward Position.
    *
    * Value range: [-100, 100]
    * Unit: percent
    **/
    velocitas::DataPointInt32 Tilt;

};

} // namespace vehicle::body::mirrors::right

#endif // VEHICLE_BODY_MIRRORS_RIGHT_RIGHT_H
