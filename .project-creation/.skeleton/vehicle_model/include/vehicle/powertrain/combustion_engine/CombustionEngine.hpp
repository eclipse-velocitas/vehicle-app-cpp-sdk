#ifndef VEHICLE_POWERTRAIN_COMBUSTION_ENGINE_COMBUSTIONENGINE_H
#define VEHICLE_POWERTRAIN_COMBUSTION_ENGINE_COMBUSTIONENGINE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/powertrain/combustion_engine/diesel_exhaust_fluid/DieselExhaustFluid.hpp"
#include "vehicle/powertrain/combustion_engine/diesel_particulate_filter/DieselParticulateFilter.hpp"

#include <string>

namespace vehicle::powertrain::combustion_engine {
using ParentClass = velocitas::Model;

/** CombustionEngine model. */
class CombustionEngine : public ParentClass {
public:

    CombustionEngine(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		AspirationType("AspirationType", this),
		Bore("Bore", this),
		CompressionRatio("CompressionRatio", this),
		Configuration("Configuration", this),
		DieselExhaustFluid("DieselExhaustFluid", this),
		DieselParticulateFilter("DieselParticulateFilter", this),
		Displacement("Displacement", this),
		ECT("ECT", this),
		EOP("EOP", this),
		EOT("EOT", this),
		EngineCode("EngineCode", this),
		EngineCoolantCapacity("EngineCoolantCapacity", this),
		EngineHours("EngineHours", this),
		EngineOilCapacity("EngineOilCapacity", this),
		EngineOilLevel("EngineOilLevel", this),
		IdleHours("IdleHours", this),
		IsRunning("IsRunning", this),
		MAF("MAF", this),
		MAP("MAP", this),
		MaxPower("MaxPower", this),
		MaxTorque("MaxTorque", this),
		NumberOfCylinders("NumberOfCylinders", this),
		NumberOfValvesPerCylinder("NumberOfValvesPerCylinder", this),
		OilLifeRemaining("OilLifeRemaining", this),
		Power("Power", this),
		Speed("Speed", this),
		StrokeLength("StrokeLength", this),
		TPS("TPS", this),
		Torque("Torque", this)
    {}

    /**
    * AspirationType: attribute (string)
    * Type of aspiration (natural, turbocharger, supercharger etc).
    *
    * Allowed values: UNKNOWN, NATURAL, SUPERCHARGER, TURBOCHARGER
    **/
    velocitas::DataPointString AspirationType;

    /**
    * Bore: attribute (float)
    * Bore in millimetres.
    *
    * Unit: mm
    **/
    velocitas::DataPointFloat Bore;

    /**
    * CompressionRatio: attribute (string)
    * Engine compression ratio, specified in the format 'X:1', e.g. '9.2:1'.
    *
    **/
    velocitas::DataPointString CompressionRatio;

    /**
    * Configuration: attribute (string)
    * Engine configuration.
    *
    * Allowed values: UNKNOWN, STRAIGHT, V, BOXER, W, ROTARY, RADIAL, SQUARE, H, U, OPPOSED, X
    **/
    velocitas::DataPointString Configuration;

    /**
    * DieselExhaustFluid: branch
    * Signals related to Diesel Exhaust Fluid (DEF). DEF is called AUS32 in ISO 22241.
    *
    * In retail and marketing other names are typically used for the fluid.
    *
    **/
    vehicle::powertrain::combustion_engine::diesel_exhaust_fluid::DieselExhaustFluid DieselExhaustFluid;

    /**
    * DieselParticulateFilter: branch
    * Diesel Particulate Filter signals.
    *
    **/
    vehicle::powertrain::combustion_engine::diesel_particulate_filter::DieselParticulateFilter DieselParticulateFilter;

    /**
    * Displacement: attribute (uint16)
    * Displacement in cubic centimetres.
    *
    * Unit: cm^3
    **/
    velocitas::DataPointUint32 Displacement;

    /**
    * ECT: sensor
    * Engine coolant temperature.
    *
    * Unit: celsius
    **/
    velocitas::DataPointInt32 ECT;

    /**
    * EOP: sensor
    * Engine oil pressure.
    *
    * Unit: kPa
    **/
    velocitas::DataPointUint32 EOP;

    /**
    * EOT: sensor
    * Engine oil temperature.
    *
    * Unit: celsius
    **/
    velocitas::DataPointInt32 EOT;

    /**
    * EngineCode: attribute (string)
    * Engine code designation, as specified by vehicle manufacturer.
    *
    * For hybrid vehicles the engine code may refer to the combination of combustion and electric engine.
    *
    **/
    velocitas::DataPointString EngineCode;

    /**
    * EngineCoolantCapacity: attribute (float)
    * Engine coolant capacity in liters.
    *
    * Unit: l
    **/
    velocitas::DataPointFloat EngineCoolantCapacity;

    /**
    * EngineHours: sensor
    * Accumulated time during engine lifetime with 'engine speed (rpm) > 0'.
    *
    * Unit: h
    **/
    velocitas::DataPointFloat EngineHours;

    /**
    * EngineOilCapacity: attribute (float)
    * Engine oil capacity in liters.
    *
    * Unit: l
    **/
    velocitas::DataPointFloat EngineOilCapacity;

    /**
    * EngineOilLevel: sensor
    * Engine oil level.
    *
    * Allowed values: CRITICALLY_LOW, LOW, NORMAL, HIGH, CRITICALLY_HIGH
    **/
    velocitas::DataPointString EngineOilLevel;

    /**
    * IdleHours: sensor
    * Accumulated idling time during engine lifetime. Definition of idling is not standardized.
    *
    * Vehicles may calculate accumulated idle time for an engine. It might be based on engine speed (rpm) below a certain limit or any other mechanism.
    *
    * Unit: h
    **/
    velocitas::DataPointFloat IdleHours;

    /**
    * IsRunning: sensor
    * Engine Running. True if engine is rotating (Speed > 0).
    *
    **/
    velocitas::DataPointBoolean IsRunning;

    /**
    * MAF: sensor
    * Grams of air drawn into engine per second.
    *
    * Unit: g/s
    **/
    velocitas::DataPointUint32 MAF;

    /**
    * MAP: sensor
    * Manifold absolute pressure possibly boosted using forced induction.
    *
    * Unit: kPa
    **/
    velocitas::DataPointUint32 MAP;

    /**
    * MaxPower: attribute (uint16)
    * Peak power, in kilowatts, that engine can generate.
    *
    * Unit: kW
    **/
    velocitas::DataPointUint32 MaxPower;

    /**
    * MaxTorque: attribute (uint16)
    * Peak torque, in newton meter, that the engine can generate.
    *
    * Unit: Nm
    **/
    velocitas::DataPointUint32 MaxTorque;

    /**
    * NumberOfCylinders: attribute (uint16)
    * Number of cylinders.
    *
    **/
    velocitas::DataPointUint32 NumberOfCylinders;

    /**
    * NumberOfValvesPerCylinder: attribute (uint16)
    * Number of valves per cylinder.
    *
    **/
    velocitas::DataPointUint32 NumberOfValvesPerCylinder;

    /**
    * OilLifeRemaining: sensor
    * Remaining engine oil life in seconds. Negative values can be used to indicate that lifetime has been exceeded.
    *
    * In addition to this a signal a vehicle can report remaining time to service (including e.g. oil change) by Vehicle.Service.TimeToService.
    *
    * Unit: s
    **/
    velocitas::DataPointInt32 OilLifeRemaining;

    /**
    * Power: sensor
    * Current engine power output. Shall be reported as 0 during engine breaking.
    *
    * Unit: kW
    **/
    velocitas::DataPointUint32 Power;

    /**
    * Speed: sensor
    * Engine speed measured as rotations per minute.
    *
    * Unit: rpm
    **/
    velocitas::DataPointUint32 Speed;

    /**
    * StrokeLength: attribute (float)
    * Stroke length in millimetres.
    *
    * Unit: mm
    **/
    velocitas::DataPointFloat StrokeLength;

    /**
    * TPS: sensor
    * Current throttle position.
    *
    * Value range: [, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 TPS;

    /**
    * Torque: sensor
    * Current engine torque. Shall be reported as 0 during engine breaking.
    *
    * During engine breaking the engine delivers a negative torque to the transmission. This negative torque shall be ignored, instead 0 shall be reported.
    *
    * Unit: Nm
    **/
    velocitas::DataPointUint32 Torque;

};

} // namespace vehicle::powertrain::combustion_engine

#endif // VEHICLE_POWERTRAIN_COMBUSTION_ENGINE_COMBUSTIONENGINE_H
