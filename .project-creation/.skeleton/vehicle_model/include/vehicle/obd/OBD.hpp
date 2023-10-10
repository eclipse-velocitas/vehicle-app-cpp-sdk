#ifndef VEHICLE_OBD_OBD_H
#define VEHICLE_OBD_OBD_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/obd/catalyst/Catalyst.hpp"
#include "vehicle/obd/drive_cycle_status/DriveCycleStatus.hpp"
#include "vehicle/obd/o_2/O2.hpp"
#include "vehicle/obd/o_2_wr/O2WR.hpp"
#include "vehicle/obd/status/Status.hpp"

#include <string>

namespace vehicle::obd {
using ParentClass = velocitas::Model;

/** OBD model. */
class OBD : public ParentClass {
public:

    OBD(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		AbsoluteLoad("AbsoluteLoad", this),
		AcceleratorPositionD("AcceleratorPositionD", this),
		AcceleratorPositionE("AcceleratorPositionE", this),
		AcceleratorPositionF("AcceleratorPositionF", this),
		AirStatus("AirStatus", this),
		AmbientAirTemperature("AmbientAirTemperature", this),
		BarometricPressure("BarometricPressure", this),
		Catalyst("Catalyst", this),
		CommandedEGR("CommandedEGR", this),
		CommandedEVAP("CommandedEVAP", this),
		CommandedEquivalenceRatio("CommandedEquivalenceRatio", this),
		ControlModuleVoltage("ControlModuleVoltage", this),
		CoolantTemperature("CoolantTemperature", this),
		DTCList("DTCList", this),
		DistanceSinceDTCClear("DistanceSinceDTCClear", this),
		DistanceWithMIL("DistanceWithMIL", this),
		DriveCycleStatus("DriveCycleStatus", this),
		EGRError("EGRError", this),
		EVAPVaporPressure("EVAPVaporPressure", this),
		EVAPVaporPressureAbsolute("EVAPVaporPressureAbsolute", this),
		EVAPVaporPressureAlternate("EVAPVaporPressureAlternate", this),
		EngineLoad("EngineLoad", this),
		EngineSpeed("EngineSpeed", this),
		EthanolPercent("EthanolPercent", this),
		FreezeDTC("FreezeDTC", this),
		FuelInjectionTiming("FuelInjectionTiming", this),
		FuelLevel("FuelLevel", this),
		FuelPressure("FuelPressure", this),
		FuelRailPressureAbsolute("FuelRailPressureAbsolute", this),
		FuelRailPressureDirect("FuelRailPressureDirect", this),
		FuelRailPressureVac("FuelRailPressureVac", this),
		FuelRate("FuelRate", this),
		FuelStatus("FuelStatus", this),
		FuelType("FuelType", this),
		HybridBatteryRemaining("HybridBatteryRemaining", this),
		IntakeTemp("IntakeTemp", this),
		IsPTOActive("IsPTOActive", this),
		LongTermFuelTrim1("LongTermFuelTrim1", this),
		LongTermFuelTrim2("LongTermFuelTrim2", this),
		LongTermO2Trim1("LongTermO2Trim1", this),
		LongTermO2Trim2("LongTermO2Trim2", this),
		LongTermO2Trim3("LongTermO2Trim3", this),
		LongTermO2Trim4("LongTermO2Trim4", this),
		MAF("MAF", this),
		MAP("MAP", this),
		MaxMAF("MaxMAF", this),
		O2("O2", this),
		O2WR("O2WR", this),
		OBDStandards("OBDStandards", this),
		OilTemperature("OilTemperature", this),
		OxygenSensorsIn2Banks("OxygenSensorsIn2Banks", this),
		OxygenSensorsIn4Banks("OxygenSensorsIn4Banks", this),
		PidsA("PidsA", this),
		PidsB("PidsB", this),
		PidsC("PidsC", this),
		RelativeAcceleratorPosition("RelativeAcceleratorPosition", this),
		RelativeThrottlePosition("RelativeThrottlePosition", this),
		RunTime("RunTime", this),
		RunTimeMIL("RunTimeMIL", this),
		ShortTermFuelTrim1("ShortTermFuelTrim1", this),
		ShortTermFuelTrim2("ShortTermFuelTrim2", this),
		ShortTermO2Trim1("ShortTermO2Trim1", this),
		ShortTermO2Trim2("ShortTermO2Trim2", this),
		ShortTermO2Trim3("ShortTermO2Trim3", this),
		ShortTermO2Trim4("ShortTermO2Trim4", this),
		Speed("Speed", this),
		Status("Status", this),
		ThrottleActuator("ThrottleActuator", this),
		ThrottlePosition("ThrottlePosition", this),
		ThrottlePositionB("ThrottlePositionB", this),
		ThrottlePositionC("ThrottlePositionC", this),
		TimeSinceDTCCleared("TimeSinceDTCCleared", this),
		TimingAdvance("TimingAdvance", this),
		WarmupsSinceDTCClear("WarmupsSinceDTCClear", this)
    {}

    /**
    * AbsoluteLoad: sensor
    * PID 43 - Absolute load value
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat AbsoluteLoad;

    /**
    * AcceleratorPositionD: sensor
    * PID 49 - Accelerator pedal position D
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat AcceleratorPositionD;

    /**
    * AcceleratorPositionE: sensor
    * PID 4A - Accelerator pedal position E
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat AcceleratorPositionE;

    /**
    * AcceleratorPositionF: sensor
    * PID 4B - Accelerator pedal position F
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat AcceleratorPositionF;

    /**
    * AirStatus: sensor
    * PID 12 - Secondary air status
    *
    **/
    velocitas::DataPointString AirStatus;

    /**
    * AmbientAirTemperature: sensor
    * PID 46 - Ambient air temperature
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat AmbientAirTemperature;

    /**
    * BarometricPressure: sensor
    * PID 33 - Barometric pressure
    *
    * Unit: kPa
    **/
    velocitas::DataPointFloat BarometricPressure;

    /**
    * Catalyst: branch
    * Catalyst signals
    *
    **/
    vehicle::obd::catalyst::Catalyst Catalyst;

    /**
    * CommandedEGR: sensor
    * PID 2C - Commanded exhaust gas recirculation (EGR)
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat CommandedEGR;

    /**
    * CommandedEVAP: sensor
    * PID 2E - Commanded evaporative purge (EVAP) valve
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat CommandedEVAP;

    /**
    * CommandedEquivalenceRatio: sensor
    * PID 44 - Commanded equivalence ratio
    *
    * Unit: ratio
    **/
    velocitas::DataPointFloat CommandedEquivalenceRatio;

    /**
    * ControlModuleVoltage: sensor
    * PID 42 - Control module voltage
    *
    * Unit: V
    **/
    velocitas::DataPointFloat ControlModuleVoltage;

    /**
    * CoolantTemperature: sensor
    * PID 05 - Coolant temperature
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat CoolantTemperature;

    /**
    * DTCList: sensor
    * List of currently active DTCs formatted according OBD II (SAE-J2012DA_201812) standard ([P|C|B|U]XXXXX )
    *
    **/
    velocitas::DataPointStringArray DTCList;

    /**
    * DistanceSinceDTCClear: sensor
    * PID 31 - Distance traveled since codes cleared
    *
    * Unit: km
    **/
    velocitas::DataPointFloat DistanceSinceDTCClear;

    /**
    * DistanceWithMIL: sensor
    * PID 21 - Distance traveled with MIL on
    *
    * Unit: km
    **/
    velocitas::DataPointFloat DistanceWithMIL;

    /**
    * DriveCycleStatus: branch
    * PID 41 - OBD status for the current drive cycle
    *
    **/
    vehicle::obd::drive_cycle_status::DriveCycleStatus DriveCycleStatus;

    /**
    * EGRError: sensor
    * PID 2D - Exhaust gas recirculation (EGR) error
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat EGRError;

    /**
    * EVAPVaporPressure: sensor
    * PID 32 - Evaporative purge (EVAP) system pressure
    *
    * Unit: Pa
    **/
    velocitas::DataPointFloat EVAPVaporPressure;

    /**
    * EVAPVaporPressureAbsolute: sensor
    * PID 53 - Absolute evaporative purge (EVAP) system pressure
    *
    * Unit: kPa
    **/
    velocitas::DataPointFloat EVAPVaporPressureAbsolute;

    /**
    * EVAPVaporPressureAlternate: sensor
    * PID 54 - Alternate evaporative purge (EVAP) system pressure
    *
    * Unit: Pa
    **/
    velocitas::DataPointFloat EVAPVaporPressureAlternate;

    /**
    * EngineLoad: sensor
    * PID 04 - Engine load in percent - 0 = no load, 100 = full load
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat EngineLoad;

    /**
    * EngineSpeed: sensor
    * PID 0C - Engine speed measured as rotations per minute
    *
    * Unit: rpm
    **/
    velocitas::DataPointFloat EngineSpeed;

    /**
    * EthanolPercent: sensor
    * PID 52 - Percentage of ethanol in the fuel
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat EthanolPercent;

    /**
    * FreezeDTC: sensor
    * PID 02 - DTC that triggered the freeze frame
    *
    **/
    velocitas::DataPointString FreezeDTC;

    /**
    * FuelInjectionTiming: sensor
    * PID 5D - Fuel injection timing
    *
    * Unit: degrees
    **/
    velocitas::DataPointFloat FuelInjectionTiming;

    /**
    * FuelLevel: sensor
    * PID 2F - Fuel level in the fuel tank
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat FuelLevel;

    /**
    * FuelPressure: sensor
    * PID 0A - Fuel pressure
    *
    * Unit: kPa
    **/
    velocitas::DataPointFloat FuelPressure;

    /**
    * FuelRailPressureAbsolute: sensor
    * PID 59 - Absolute fuel rail pressure
    *
    * Unit: kPa
    **/
    velocitas::DataPointFloat FuelRailPressureAbsolute;

    /**
    * FuelRailPressureDirect: sensor
    * PID 23 - Fuel rail pressure direct inject
    *
    * Unit: kPa
    **/
    velocitas::DataPointFloat FuelRailPressureDirect;

    /**
    * FuelRailPressureVac: sensor
    * PID 22 - Fuel rail pressure relative to vacuum
    *
    * Unit: kPa
    **/
    velocitas::DataPointFloat FuelRailPressureVac;

    /**
    * FuelRate: sensor
    * PID 5E - Engine fuel rate
    *
    * Unit: l/h
    **/
    velocitas::DataPointFloat FuelRate;

    /**
    * FuelStatus: sensor
    * PID 03 - Fuel status
    *
    **/
    velocitas::DataPointString FuelStatus;

    /**
    * FuelType: sensor
    * PID 51 - Fuel type
    *
    **/
    velocitas::DataPointString FuelType;

    /**
    * HybridBatteryRemaining: sensor
    * PID 5B - Remaining life of hybrid battery
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat HybridBatteryRemaining;

    /**
    * IntakeTemp: sensor
    * PID 0F - Intake temperature
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat IntakeTemp;

    /**
    * IsPTOActive: sensor
    * PID 1E - Auxiliary input status (power take off)
    *
    **/
    velocitas::DataPointBoolean IsPTOActive;

    /**
    * LongTermFuelTrim1: sensor
    * PID 07 - Long Term (learned) Fuel Trim - Bank 1 - negative percent leaner, positive percent richer
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat LongTermFuelTrim1;

    /**
    * LongTermFuelTrim2: sensor
    * PID 09 - Long Term (learned) Fuel Trim - Bank 2 - negative percent leaner, positive percent richer
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat LongTermFuelTrim2;

    /**
    * LongTermO2Trim1: sensor
    * PID 56 (byte A) - Long term secondary O2 trim - Bank 1
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat LongTermO2Trim1;

    /**
    * LongTermO2Trim2: sensor
    * PID 58 (byte A) - Long term secondary O2 trim - Bank 2
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat LongTermO2Trim2;

    /**
    * LongTermO2Trim3: sensor
    * PID 56 (byte B) - Long term secondary O2 trim - Bank 3
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat LongTermO2Trim3;

    /**
    * LongTermO2Trim4: sensor
    * PID 58 (byte B) - Long term secondary O2 trim - Bank 4
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat LongTermO2Trim4;

    /**
    * MAF: sensor
    * PID 10 - Grams of air drawn into engine per second
    *
    * Unit: g/s
    **/
    velocitas::DataPointFloat MAF;

    /**
    * MAP: sensor
    * PID 0B - Intake manifold pressure
    *
    * Unit: kPa
    **/
    velocitas::DataPointFloat MAP;

    /**
    * MaxMAF: sensor
    * PID 50 - Maximum flow for mass air flow sensor
    *
    * Unit: g/s
    **/
    velocitas::DataPointFloat MaxMAF;

    /**
    * O2: branch
    * Oxygen sensors (PID 14 - PID 1B)
    *
    **/
    vehicle::obd::o_2::O2 O2;

    /**
    * O2WR: branch
    * Wide range/band oxygen sensors (PID 24 - 2B and PID 34 - 3B)
    *
    **/
    vehicle::obd::o_2_wr::O2WR O2WR;

    /**
    * OBDStandards: attribute (uint8)
    * PID 1C - OBD standards this vehicle conforms to
    *
    **/
    velocitas::DataPointUint32 OBDStandards;

    /**
    * OilTemperature: sensor
    * PID 5C - Engine oil temperature
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat OilTemperature;

    /**
    * OxygenSensorsIn2Banks: sensor
    * PID 13 - Presence of oxygen sensors in 2 banks. [A0..A3] == Bank 1, Sensors 1-4. [A4..A7] == Bank 2, Sensors 1-4
    *
    **/
    velocitas::DataPointUint32 OxygenSensorsIn2Banks;

    /**
    * OxygenSensorsIn4Banks: sensor
    * PID 1D - Presence of oxygen sensors in 4 banks. Similar to PID 13, but [A0..A7] == [B1S1, B1S2, B2S1, B2S2, B3S1, B3S2, B4S1, B4S2]
    *
    **/
    velocitas::DataPointUint32 OxygenSensorsIn4Banks;

    /**
    * PidsA: sensor
    * PID 00 - Bit array of the supported pids 01 to 20
    *
    **/
    velocitas::DataPointUint32 PidsA;

    /**
    * PidsB: sensor
    * PID 20 - Bit array of the supported pids 21 to 40
    *
    **/
    velocitas::DataPointUint32 PidsB;

    /**
    * PidsC: sensor
    * PID 40 - Bit array of the supported pids 41 to 60
    *
    **/
    velocitas::DataPointUint32 PidsC;

    /**
    * RelativeAcceleratorPosition: sensor
    * PID 5A - Relative accelerator pedal position
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat RelativeAcceleratorPosition;

    /**
    * RelativeThrottlePosition: sensor
    * PID 45 - Relative throttle position
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat RelativeThrottlePosition;

    /**
    * RunTime: sensor
    * PID 1F - Engine run time
    *
    * Unit: s
    **/
    velocitas::DataPointFloat RunTime;

    /**
    * RunTimeMIL: sensor
    * PID 4D - Run time with MIL on
    *
    * Unit: min
    **/
    velocitas::DataPointFloat RunTimeMIL;

    /**
    * ShortTermFuelTrim1: sensor
    * PID 06 - Short Term (immediate) Fuel Trim - Bank 1 - negative percent leaner, positive percent richer
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat ShortTermFuelTrim1;

    /**
    * ShortTermFuelTrim2: sensor
    * PID 08 - Short Term (immediate) Fuel Trim - Bank 2 - negative percent leaner, positive percent richer
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat ShortTermFuelTrim2;

    /**
    * ShortTermO2Trim1: sensor
    * PID 55 (byte A) - Short term secondary O2 trim - Bank 1
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat ShortTermO2Trim1;

    /**
    * ShortTermO2Trim2: sensor
    * PID 57 (byte A) - Short term secondary O2 trim - Bank 2
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat ShortTermO2Trim2;

    /**
    * ShortTermO2Trim3: sensor
    * PID 55 (byte B) - Short term secondary O2 trim - Bank 3
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat ShortTermO2Trim3;

    /**
    * ShortTermO2Trim4: sensor
    * PID 57 (byte B) - Short term secondary O2 trim - Bank 4
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat ShortTermO2Trim4;

    /**
    * Speed: sensor
    * PID 0D - Vehicle speed
    *
    * Unit: km/h
    **/
    velocitas::DataPointFloat Speed;

    /**
    * Status: branch
    * PID 01 - OBD status
    *
    **/
    vehicle::obd::status::Status Status;

    /**
    * ThrottleActuator: sensor
    * PID 4C - Commanded throttle actuator
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat ThrottleActuator;

    /**
    * ThrottlePosition: sensor
    * PID 11 - Throttle position - 0 = closed throttle, 100 = open throttle
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat ThrottlePosition;

    /**
    * ThrottlePositionB: sensor
    * PID 47 - Absolute throttle position B
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat ThrottlePositionB;

    /**
    * ThrottlePositionC: sensor
    * PID 48 - Absolute throttle position C
    *
    * Unit: percent
    **/
    velocitas::DataPointFloat ThrottlePositionC;

    /**
    * TimeSinceDTCCleared: sensor
    * PID 4E - Time since trouble codes cleared
    *
    * Unit: min
    **/
    velocitas::DataPointFloat TimeSinceDTCCleared;

    /**
    * TimingAdvance: sensor
    * PID 0E - Time advance
    *
    * Unit: degrees
    **/
    velocitas::DataPointFloat TimingAdvance;

    /**
    * WarmupsSinceDTCClear: sensor
    * PID 30 - Number of warm-ups since codes cleared
    *
    **/
    velocitas::DataPointUint32 WarmupsSinceDTCClear;

};

} // namespace vehicle::obd

#endif // VEHICLE_OBD_OBD_H
