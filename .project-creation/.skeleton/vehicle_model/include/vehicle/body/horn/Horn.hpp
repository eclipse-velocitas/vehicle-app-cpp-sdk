#ifndef VEHICLE_BODY_HORN_HORN_H
#define VEHICLE_BODY_HORN_HORN_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::body::horn {
using ParentClass = velocitas::Model;

/** Horn model. */
class Horn : public ParentClass {
public:

    Horn(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsActive("IsActive", this)
    {}

    /**
    * IsActive: actuator
    * Horn active or inactive. True = Active. False = Inactive.
    *
    **/
    velocitas::DataPointBoolean IsActive;

};

} // namespace vehicle::body::horn

#endif // VEHICLE_BODY_HORN_HORN_H
