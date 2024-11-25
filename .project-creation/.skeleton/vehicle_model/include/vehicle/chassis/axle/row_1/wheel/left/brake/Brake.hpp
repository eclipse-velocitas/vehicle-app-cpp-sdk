#ifndef VEHICLE_CHASSIS_AXLE_ROW_1_WHEEL_LEFT_BRAKE_BRAKE_H
#define VEHICLE_CHASSIS_AXLE_ROW_1_WHEEL_LEFT_BRAKE_BRAKE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::chassis::axle::row_1::wheel::left::brake {
using ParentClass = velocitas::Model;

/** Brake model. */
class Brake : public ParentClass {
public:

    Brake(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		FluidLevel("FluidLevel", this),
		IsBrakesWorn("IsBrakesWorn", this),
		IsFluidLevelLow("IsFluidLevelLow", this),
		PadWear("PadWear", this)
    {}

    /**
    * FluidLevel: sensor
    * Brake fluid level as percent. 0 = Empty. 100 = Full.
    *
    * Value range: [, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 FluidLevel;

    /**
    * IsBrakesWorn: sensor
    * Brake pad wear status. True = Worn. False = Not Worn.
    *
    **/
    velocitas::DataPointBoolean IsBrakesWorn;

    /**
    * IsFluidLevelLow: sensor
    * Brake fluid level status. True = Brake fluid level low. False = Brake fluid level OK.
    *
    **/
    velocitas::DataPointBoolean IsFluidLevelLow;

    /**
    * PadWear: sensor
    * Brake pad wear as percent. 0 = No Wear. 100 = Worn.
    *
    * Value range: [, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 PadWear;

};

} // namespace vehicle::chassis::axle::row_1::wheel::left::brake

#endif // VEHICLE_CHASSIS_AXLE_ROW_1_WHEEL_LEFT_BRAKE_BRAKE_H
