#ifndef VEHICLE_BODY_TRUNK_TRUNK_H
#define VEHICLE_BODY_TRUNK_TRUNK_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/body/trunk/front/Front.hpp"
#include "vehicle/body/trunk/rear/Rear.hpp"

#include <string>

namespace vehicle::body::trunk {
using ParentClass = velocitas::Model;

/** Trunk model. */
class Trunk : public ParentClass {
public:

    Trunk(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Front("Front", this),
		Rear("Rear", this)
    {}

    /**
    * Front: branch
    * Trunk status.
    *
    **/
    vehicle::body::trunk::front::Front Front;

    /**
    * Rear: branch
    * Trunk status.
    *
    **/
    vehicle::body::trunk::rear::Rear Rear;

};

} // namespace vehicle::body::trunk

#endif // VEHICLE_BODY_TRUNK_TRUNK_H
