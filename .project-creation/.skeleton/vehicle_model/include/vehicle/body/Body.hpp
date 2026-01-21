#ifndef VEHICLE_BODY_BODY_H
#define VEHICLE_BODY_BODY_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/body/hood/Hood.hpp"
#include "vehicle/body/horn/Horn.hpp"
#include "vehicle/body/lights/Lights.hpp"
#include "vehicle/body/mirrors/Mirrors.hpp"
#include "vehicle/body/raindetection/Raindetection.hpp"
#include "vehicle/body/trunk/Trunk.hpp"
#include "vehicle/body/windshield/Windshield.hpp"

#include <string>

namespace vehicle::body {
using ParentClass = velocitas::Model;

/** Body model. */
class Body : public ParentClass {
public:

    Body(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		BodyType("BodyType", this),
		Hood("Hood", this),
		Horn("Horn", this),
		Lights("Lights", this),
		Mirrors("Mirrors", this),
		Raindetection("Raindetection", this),
		RearMainSpoilerPosition("RearMainSpoilerPosition", this),
		RefuelPosition("RefuelPosition", this),
		Trunk("Trunk", this),
		Windshield("Windshield", this)
    {}

    /**
    * BodyType: attribute (string)
    * Body type code as defined by ISO 3779.
    *
    **/
    velocitas::DataPointString BodyType;

    /**
    * Hood: branch
    * Hood status.
    *
    **/
    vehicle::body::hood::Hood Hood;

    /**
    * Horn: branch
    * Horn signals.
    *
    **/
    vehicle::body::horn::Horn Horn;

    /**
    * Lights: branch
    * All lights.
    *
    **/
    vehicle::body::lights::Lights Lights;

    /**
    * Mirrors: branch
    * All mirrors.
    *
    **/
    vehicle::body::mirrors::Mirrors Mirrors;

    /**
    * Raindetection: branch
    * Rainsensor signals.
    *
    **/
    vehicle::body::raindetection::Raindetection Raindetection;

    /**
    * RearMainSpoilerPosition: actuator
    * Rear spoiler position, 0% = Spoiler fully stowed. 100% = Spoiler fully exposed.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat RearMainSpoilerPosition;

    /**
    * RefuelPosition: attribute (string)
    * Location of the fuel cap or charge port.
    *
    * Allowed values: FRONT_LEFT, FRONT_RIGHT, MIDDLE_LEFT, MIDDLE_RIGHT, REAR_LEFT, REAR_RIGHT
    **/
    velocitas::DataPointString RefuelPosition;

    /**
    * Trunk: branch
    * Trunk status.
    *
    **/
    vehicle::body::trunk::Trunk Trunk;

    /**
    * Windshield: branch
    * Windshield signals.
    *
    **/
    vehicle::body::windshield::Windshield Windshield;

};

} // namespace vehicle::body

#endif // VEHICLE_BODY_BODY_H
