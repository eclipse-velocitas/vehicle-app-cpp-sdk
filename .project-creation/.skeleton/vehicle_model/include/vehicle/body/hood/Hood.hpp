#ifndef VEHICLE_BODY_HOOD_HOOD_H
#define VEHICLE_BODY_HOOD_HOOD_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::body::hood {
using ParentClass = velocitas::Model;

/** Hood model. */
class Hood : public ParentClass {
public:

    Hood(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsOpen("IsOpen", this)
    {}

    /**
    * IsOpen: actuator
    * Hood open or closed. True = Open. False = Closed.
    *
    **/
    velocitas::DataPointBoolean IsOpen;

};

} // namespace vehicle::body::hood

#endif // VEHICLE_BODY_HOOD_HOOD_H
