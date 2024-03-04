#ifndef VEHICLE_BODY_TRUNK_REAR_REAR_H
#define VEHICLE_BODY_TRUNK_REAR_REAR_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::body::trunk::rear {
using ParentClass = velocitas::Model;

/** Rear model. */
class Rear : public ParentClass {
public:

    Rear(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsLocked("IsLocked", this),
		IsOpen("IsOpen", this)
    {}

    /**
    * IsLocked: actuator
    * Is trunk locked or unlocked. True = Locked. False = Unlocked.
    *
    **/
    velocitas::DataPointBoolean IsLocked;

    /**
    * IsOpen: actuator
    * Trunk open or closed. True = Open. False = Closed.
    *
    **/
    velocitas::DataPointBoolean IsOpen;

};

} // namespace vehicle::body::trunk::rear

#endif // VEHICLE_BODY_TRUNK_REAR_REAR_H
