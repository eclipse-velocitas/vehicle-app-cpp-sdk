#ifndef VEHICLE_CABIN_LIGHTS_LIGHTS_H
#define VEHICLE_CABIN_LIGHTS_LIGHTS_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/lights/spotlight/Spotlight.hpp"

#include <string>

namespace vehicle::cabin::lights {
using ParentClass = velocitas::Model;

/** Lights model. */
class Lights : public ParentClass {
public:

    Lights(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		AmbientLight("AmbientLight", this),
		IsDomeOn("IsDomeOn", this),
		IsGloveBoxOn("IsGloveBoxOn", this),
		IsTrunkOn("IsTrunkOn", this),
		LightIntensity("LightIntensity", this),
		Spotlight("Spotlight", this)
    {}

    /**
    * AmbientLight: sensor
    * How much ambient light is detected in cabin. 0 = No ambient light. 100 = Full brightness
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 AmbientLight;

    /**
    * IsDomeOn: actuator
    * Is central dome light light on
    *
    **/
    velocitas::DataPointBoolean IsDomeOn;

    /**
    * IsGloveBoxOn: actuator
    * Is glove box light on
    *
    **/
    velocitas::DataPointBoolean IsGloveBoxOn;

    /**
    * IsTrunkOn: actuator
    * Is trunk light light on
    *
    **/
    velocitas::DataPointBoolean IsTrunkOn;

    /**
    * LightIntensity: sensor
    * Intensity of the interior lights. 0 = Off. 100 = Full brightness.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 LightIntensity;

    /**
    * Spotlight: branch
    * Spotlight for a specific area in the vehicle.
    *
    **/
    vehicle::cabin::lights::spotlight::Spotlight Spotlight;

};

} // namespace vehicle::cabin::lights

#endif // VEHICLE_CABIN_LIGHTS_LIGHTS_H
