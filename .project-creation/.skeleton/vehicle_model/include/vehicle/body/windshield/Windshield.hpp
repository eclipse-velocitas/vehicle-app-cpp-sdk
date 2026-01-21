#ifndef VEHICLE_BODY_WINDSHIELD_WINDSHIELD_H
#define VEHICLE_BODY_WINDSHIELD_WINDSHIELD_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/body/windshield/front/Front.hpp"
#include "vehicle/body/windshield/rear/Rear.hpp"

#include <string>

namespace vehicle::body::windshield {
using ParentClass = velocitas::Model;

/** Windshield model. */
class Windshield : public ParentClass {
public:

    Windshield(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Front("Front", this),
		Rear("Rear", this)
    {}

    /**
    * Front: branch
    * Windshield signals.
    *
    **/
    vehicle::body::windshield::front::Front Front;

    /**
    * Rear: branch
    * Windshield signals.
    *
    **/
    vehicle::body::windshield::rear::Rear Rear;

};

} // namespace vehicle::body::windshield

#endif // VEHICLE_BODY_WINDSHIELD_WINDSHIELD_H
