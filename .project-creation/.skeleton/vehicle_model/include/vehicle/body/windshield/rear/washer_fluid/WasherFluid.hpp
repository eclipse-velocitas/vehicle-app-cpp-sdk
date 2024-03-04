#ifndef VEHICLE_BODY_WINDSHIELD_REAR_WASHER_FLUID_WASHERFLUID_H
#define VEHICLE_BODY_WINDSHIELD_REAR_WASHER_FLUID_WASHERFLUID_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::body::windshield::rear::washer_fluid {
using ParentClass = velocitas::Model;

/** WasherFluid model. */
class WasherFluid : public ParentClass {
public:

    WasherFluid(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsLevelLow("IsLevelLow", this),
		Level("Level", this)
    {}

    /**
    * IsLevelLow: sensor
    * Low level indication for washer fluid. True = Level Low. False = Level OK.
    *
    **/
    velocitas::DataPointBoolean IsLevelLow;

    /**
    * Level: sensor
    * Washer fluid level as a percent. 0 = Empty. 100 = Full.
    *
    * Value range: [, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 Level;

};

} // namespace vehicle::body::windshield::rear::washer_fluid

#endif // VEHICLE_BODY_WINDSHIELD_REAR_WASHER_FLUID_WASHERFLUID_H
