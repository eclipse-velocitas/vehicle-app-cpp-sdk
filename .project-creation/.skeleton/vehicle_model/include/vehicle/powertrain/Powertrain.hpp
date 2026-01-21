#ifndef VEHICLE_POWERTRAIN_POWERTRAIN_H
#define VEHICLE_POWERTRAIN_POWERTRAIN_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/powertrain/combustion_engine/CombustionEngine.hpp"
#include "vehicle/powertrain/electric_motor/ElectricMotor.hpp"
#include "vehicle/powertrain/fuel_system/FuelSystem.hpp"
#include "vehicle/powertrain/traction_battery/TractionBattery.hpp"
#include "vehicle/powertrain/transmission/Transmission.hpp"

#include <string>

namespace vehicle::powertrain {
using ParentClass = velocitas::Model;

/** Powertrain model. */
class Powertrain : public ParentClass {
public:

    Powertrain(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		AccumulatedBrakingEnergy("AccumulatedBrakingEnergy", this),
		CombustionEngine("CombustionEngine", this),
		ElectricMotor("ElectricMotor", this),
		FuelSystem("FuelSystem", this),
		Range("Range", this),
		TractionBattery("TractionBattery", this),
		Transmission("Transmission", this),
		Type("Type", this)
    {}

    /**
    * AccumulatedBrakingEnergy: sensor
    * The accumulated energy from regenerative braking over lifetime.
    *
    * Unit: kWh
    **/
    velocitas::DataPointFloat AccumulatedBrakingEnergy;

    /**
    * CombustionEngine: branch
    * Engine-specific data, stopping at the bell housing.
    *
    **/
    vehicle::powertrain::combustion_engine::CombustionEngine CombustionEngine;

    /**
    * ElectricMotor: branch
    * Electric Motor specific data.
    *
    **/
    vehicle::powertrain::electric_motor::ElectricMotor ElectricMotor;

    /**
    * FuelSystem: branch
    * Fuel system data.
    *
    **/
    vehicle::powertrain::fuel_system::FuelSystem FuelSystem;

    /**
    * Range: sensor
    * Remaining range in meters using all energy sources available in the vehicle.
    *
    * Unit: m
    **/
    velocitas::DataPointUint32 Range;

    /**
    * TractionBattery: branch
    * Battery Management data.
    *
    **/
    vehicle::powertrain::traction_battery::TractionBattery TractionBattery;

    /**
    * Transmission: branch
    * Transmission-specific data, stopping at the drive shafts.
    *
    **/
    vehicle::powertrain::transmission::Transmission Transmission;

    /**
    * Type: attribute (string)
    * Defines the powertrain type of the vehicle.
    *
    * For vehicles with a combustion engine (including hybrids) more detailed information on fuels supported can be found in FuelSystem.SupportedFuelTypes and FuelSystem.SupportedFuels.
    *
    * Allowed values: COMBUSTION, HYBRID, ELECTRIC
    **/
    velocitas::DataPointString Type;

};

} // namespace vehicle::powertrain

#endif // VEHICLE_POWERTRAIN_POWERTRAIN_H
