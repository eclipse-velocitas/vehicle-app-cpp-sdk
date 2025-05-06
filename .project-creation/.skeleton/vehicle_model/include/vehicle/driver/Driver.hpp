#ifndef VEHICLE_DRIVER_DRIVER_H
#define VEHICLE_DRIVER_DRIVER_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/driver/identifier/Identifier.hpp"

#include <string>

namespace vehicle::driver {
using ParentClass = velocitas::Model;

/** Driver model. */
class Driver : public ParentClass {
public:

    Driver(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		AttentiveProbability("AttentiveProbability", this),
		DistractionLevel("DistractionLevel", this),
		FatigueLevel("FatigueLevel", this),
		HeartRate("HeartRate", this),
		Identifier("Identifier", this),
		IsEyesOnRoad("IsEyesOnRoad", this)
    {}

    /**
    * AttentiveProbability: sensor
    * Probability of attentiveness of the driver.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat AttentiveProbability;

    /**
    * DistractionLevel: sensor
    * Distraction level of the driver will be the level how much the driver is distracted, by multiple factors. E.g. Driving situation, acustical or optical signales inside the cockpit, phone calls.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat DistractionLevel;

    /**
    * FatigueLevel: sensor
    * Fatigueness level of driver. Evaluated by multiple factors like trip time, behaviour of steering, eye status.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat FatigueLevel;

    /**
    * HeartRate: sensor
    * Heart rate of the driver.
    *
    **/
    velocitas::DataPointUint32 HeartRate;

    /**
    * Identifier: branch
    * Identifier attributes based on OAuth 2.0.
    *
    **/
    vehicle::driver::identifier::Identifier Identifier;

    /**
    * IsEyesOnRoad: sensor
    * Has driver the eyes on road or not?
    *
    **/
    velocitas::DataPointBoolean IsEyesOnRoad;

};

} // namespace vehicle::driver

#endif // VEHICLE_DRIVER_DRIVER_H
