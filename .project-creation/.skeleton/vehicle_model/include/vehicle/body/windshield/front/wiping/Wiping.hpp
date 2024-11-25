#ifndef VEHICLE_BODY_WINDSHIELD_FRONT_WIPING_WIPING_H
#define VEHICLE_BODY_WINDSHIELD_FRONT_WIPING_WIPING_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/body/windshield/front/wiping/system/System.hpp"

#include <string>

namespace vehicle::body::windshield::front::wiping {
using ParentClass = velocitas::Model;

/** Wiping model. */
class Wiping : public ParentClass {
public:

    Wiping(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Intensity("Intensity", this),
		IsWipersWorn("IsWipersWorn", this),
		Mode("Mode", this),
		System("System", this),
		WiperWear("WiperWear", this)
    {}

    /**
    * Intensity: actuator
    * Relative intensity/sensitivity for interval and rain sensor mode as requested by user/driver. Has no significance if Windshield.Wiping.Mode is OFF/SLOW/MEDIUM/FAST 0 - wipers inactive. 1 - minimum intensity (lowest frequency/sensitivity, longest interval). 2/3/4/... - higher intensity (higher frequency/sensitivity, shorter interval). Maximum value supported is vehicle specific.
    *
    **/
    velocitas::DataPointUint32 Intensity;

    /**
    * IsWipersWorn: sensor
    * Wiper wear status. True = Worn, Replacement recommended or required. False = Not Worn.
    *
    **/
    velocitas::DataPointBoolean IsWipersWorn;

    /**
    * Mode: actuator
    * Wiper mode requested by user/driver. INTERVAL indicates intermittent wiping, with fixed time interval between each wipe. RAIN_SENSOR indicates intermittent wiping based on rain intensity.
    *
    * Allowed values: OFF, SLOW, MEDIUM, FAST, INTERVAL, RAIN_SENSOR
    **/
    velocitas::DataPointString Mode;

    /**
    * System: branch
    * Signals to control behavior of wipers in detail. By default VSS expects only one instance.
    *
    * These signals are typically not directly available to the user/driver of the vehicle. The overlay in overlays/extensions/dual_wiper_systems.vspec can be used to modify this branch to support two instances; Primary and Secondary.
    *
    **/
    vehicle::body::windshield::front::wiping::system::System System;

    /**
    * WiperWear: sensor
    * Wiper wear as percent. 0 = No Wear. 100 = Worn. Replacement required. Method for calculating or estimating wiper wear is vehicle specific. For windshields with multiple wipers the wear reported shall correspond to the most worn wiper.
    *
    * Value range: [, 100]
    **/
    velocitas::DataPointUint32 WiperWear;

};

} // namespace vehicle::body::windshield::front::wiping

#endif // VEHICLE_BODY_WINDSHIELD_FRONT_WIPING_WIPING_H
