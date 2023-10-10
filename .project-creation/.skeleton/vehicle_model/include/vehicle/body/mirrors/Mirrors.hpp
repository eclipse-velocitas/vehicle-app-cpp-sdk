#ifndef VEHICLE_BODY_MIRRORS_MIRRORS_H
#define VEHICLE_BODY_MIRRORS_MIRRORS_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/body/mirrors/left/Left.hpp"
#include "vehicle/body/mirrors/right/Right.hpp"

#include <string>

namespace vehicle::body::mirrors {
using ParentClass = velocitas::Model;

/** Mirrors model. */
class Mirrors : public ParentClass {
public:

    Mirrors(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Left("Left", this),
		Right("Right", this)
    {}

    /**
    * Left: branch
    * All mirrors.
    *
    **/
    vehicle::body::mirrors::left::Left Left;

    /**
    * Right: branch
    * All mirrors.
    *
    **/
    vehicle::body::mirrors::right::Right Right;

};

} // namespace vehicle::body::mirrors

#endif // VEHICLE_BODY_MIRRORS_MIRRORS_H
