#ifndef VEHICLE_POWERTRAIN_COMBUSTION_ENGINE_DIESEL_EXHAUST_FLUID_DIESELEXHAUSTFLUID_H
#define VEHICLE_POWERTRAIN_COMBUSTION_ENGINE_DIESEL_EXHAUST_FLUID_DIESELEXHAUSTFLUID_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::powertrain::combustion_engine::diesel_exhaust_fluid {
using ParentClass = velocitas::Model;

/** DieselExhaustFluid model. */
class DieselExhaustFluid : public ParentClass {
public:

    DieselExhaustFluid(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Capacity("Capacity", this),
		IsLevelLow("IsLevelLow", this),
		Level("Level", this),
		Range("Range", this)
    {}

    /**
    * Capacity: attribute (float)
    * Capacity in liters of the Diesel Exhaust Fluid Tank.
    *
    * Unit: l
    **/
    velocitas::DataPointFloat Capacity;

    /**
    * IsLevelLow: sensor
    * Indicates if the Diesel Exhaust Fluid level is low. True if level is low. Definition of low is vehicle dependent.
    *
    **/
    velocitas::DataPointBoolean IsLevelLow;

    /**
    * Level: sensor
    * Level of the Diesel Exhaust Fluid tank as percent of capacity. 0 = empty. 100 = full.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 Level;

    /**
    * Range: sensor
    * Remaining range in meters of the Diesel Exhaust Fluid present in the vehicle.
    *
    * Unit: m
    **/
    velocitas::DataPointUint32 Range;

};

} // namespace vehicle::powertrain::combustion_engine::diesel_exhaust_fluid

#endif // VEHICLE_POWERTRAIN_COMBUSTION_ENGINE_DIESEL_EXHAUST_FLUID_DIESELEXHAUSTFLUID_H
