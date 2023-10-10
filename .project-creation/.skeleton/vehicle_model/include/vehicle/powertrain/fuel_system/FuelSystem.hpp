#ifndef VEHICLE_POWERTRAIN_FUEL_SYSTEM_FUELSYSTEM_H
#define VEHICLE_POWERTRAIN_FUEL_SYSTEM_FUELSYSTEM_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::powertrain::fuel_system {
using ParentClass = velocitas::Model;

/** FuelSystem model. */
class FuelSystem : public ParentClass {
public:

    FuelSystem(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		AverageConsumption("AverageConsumption", this),
		ConsumptionSinceStart("ConsumptionSinceStart", this),
		HybridType("HybridType", this),
		InstantConsumption("InstantConsumption", this),
		IsEngineStopStartEnabled("IsEngineStopStartEnabled", this),
		IsFuelLevelLow("IsFuelLevelLow", this),
		Level("Level", this),
		Range("Range", this),
		SupportedFuel("SupportedFuel", this),
		SupportedFuelTypes("SupportedFuelTypes", this),
		TankCapacity("TankCapacity", this),
		TimeSinceStart("TimeSinceStart", this)
    {}

    /**
    * AverageConsumption: sensor
    * Average consumption in liters per 100 km.
    *
    * Value range: [0, ]
    * Unit: l/100km
    **/
    velocitas::DataPointFloat AverageConsumption;

    /**
    * ConsumptionSinceStart: sensor
    * Fuel amount in liters consumed since start of current trip.
    *
    * Unit: l
    **/
    velocitas::DataPointFloat ConsumptionSinceStart;

    /**
    * HybridType: attribute (string)
    * Defines the hybrid type of the vehicle.
    *
    * Allowed values: UNKNOWN, NOT_APPLICABLE, STOP_START, BELT_ISG, CIMG, PHEV
    **/
    velocitas::DataPointString HybridType;

    /**
    * InstantConsumption: sensor
    * Current consumption in liters per 100 km.
    *
    * Value range: [0, ]
    * Unit: l/100km
    **/
    velocitas::DataPointFloat InstantConsumption;

    /**
    * IsEngineStopStartEnabled: sensor
    * Indicates whether eco start stop is currently enabled.
    *
    **/
    velocitas::DataPointBoolean IsEngineStopStartEnabled;

    /**
    * IsFuelLevelLow: sensor
    * Indicates that the fuel level is low (e.g. <50km range).
    *
    **/
    velocitas::DataPointBoolean IsFuelLevelLow;

    /**
    * Level: sensor
    * Level in fuel tank as percent of capacity. 0 = empty. 100 = full.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 Level;

    /**
    * Range: sensor
    * Remaining range in meters using only liquid fuel.
    *
    * Unit: m
    **/
    velocitas::DataPointUint32 Range;

    /**
    * SupportedFuel: attribute (string[])
    * Detailed information on fuels supported by the vehicle. Identifiers originating from DIN EN 16942:2021-08, appendix B, with additional suffix for octane (RON) where relevant.
    *
    * RON 95 is sometimes referred to as Super, RON 98 as Super Plus.
    *
    * Allowed values: E5_95, E5_98, E10_95, E10_98, E85, B7, B10, B20, B30, B100, XTL, LPG, CNG, LNG, H2, OTHER
    **/
    velocitas::DataPointStringArray SupportedFuel;

    /**
    * SupportedFuelTypes: attribute (string[])
    * High level information of fuel types supported
    *
    * If a vehicle also has an electric drivetrain (e.g. hybrid) that will be obvious from the PowerTrain.Type signal.
    *
    * Allowed values: GASOLINE, DIESEL, E85, LPG, CNG, LNG, H2, OTHER
    **/
    velocitas::DataPointStringArray SupportedFuelTypes;

    /**
    * TankCapacity: attribute (float)
    * Capacity of the fuel tank in liters.
    *
    * Unit: l
    **/
    velocitas::DataPointFloat TankCapacity;

    /**
    * TimeSinceStart: sensor
    * Time in seconds elapsed since start of current trip.
    *
    * Unit: s
    **/
    velocitas::DataPointUint32 TimeSinceStart;

};

} // namespace vehicle::powertrain::fuel_system

#endif // VEHICLE_POWERTRAIN_FUEL_SYSTEM_FUELSYSTEM_H
