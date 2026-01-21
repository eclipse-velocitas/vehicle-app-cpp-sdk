#ifndef VEHICLE_POWERTRAIN_TRACTION_BATTERY_TRACTIONBATTERY_H
#define VEHICLE_POWERTRAIN_TRACTION_BATTERY_TRACTIONBATTERY_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/powertrain/traction_battery/charging/Charging.hpp"
#include "vehicle/powertrain/traction_battery/dcdc/DCDC.hpp"
#include "vehicle/powertrain/traction_battery/state_of_charge/StateOfCharge.hpp"
#include "vehicle/powertrain/traction_battery/temperature/Temperature.hpp"

#include <string>

namespace vehicle::powertrain::traction_battery {
using ParentClass = velocitas::Model;

/** TractionBattery model. */
class TractionBattery : public ParentClass {
public:

    TractionBattery(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		AccumulatedChargedEnergy("AccumulatedChargedEnergy", this),
		AccumulatedChargedThroughput("AccumulatedChargedThroughput", this),
		AccumulatedConsumedEnergy("AccumulatedConsumedEnergy", this),
		AccumulatedConsumedThroughput("AccumulatedConsumedThroughput", this),
		Charging("Charging", this),
		CurrentCurrent("CurrentCurrent", this),
		CurrentPower("CurrentPower", this),
		CurrentVoltage("CurrentVoltage", this),
		DCDC("DCDC", this),
		GrossCapacity("GrossCapacity", this),
		Id("Id", this),
		IsGroundConnected("IsGroundConnected", this),
		IsPowerConnected("IsPowerConnected", this),
		MaxVoltage("MaxVoltage", this),
		NetCapacity("NetCapacity", this),
		NominalVoltage("NominalVoltage", this),
		PowerLoss("PowerLoss", this),
		ProductionDate("ProductionDate", this),
		Range("Range", this),
		StateOfCharge("StateOfCharge", this),
		StateOfHealth("StateOfHealth", this),
		Temperature("Temperature", this)
    {}

    /**
    * AccumulatedChargedEnergy: sensor
    * The accumulated energy delivered to the battery during charging over lifetime of the battery.
    *
    * Unit: kWh
    **/
    velocitas::DataPointFloat AccumulatedChargedEnergy;

    /**
    * AccumulatedChargedThroughput: sensor
    * The accumulated charge throughput delivered to the battery during charging over lifetime of the battery.
    *
    * Unit: Ah
    **/
    velocitas::DataPointFloat AccumulatedChargedThroughput;

    /**
    * AccumulatedConsumedEnergy: sensor
    * The accumulated energy leaving HV battery for propulsion and auxiliary loads over lifetime of the battery.
    *
    * Unit: kWh
    **/
    velocitas::DataPointFloat AccumulatedConsumedEnergy;

    /**
    * AccumulatedConsumedThroughput: sensor
    * The accumulated charge throughput leaving HV battery for propulsion and auxiliary loads over lifetime of the battery.
    *
    * Unit: Ah
    **/
    velocitas::DataPointFloat AccumulatedConsumedThroughput;

    /**
    * Charging: branch
    * Properties related to battery charging.
    *
    **/
    vehicle::powertrain::traction_battery::charging::Charging Charging;

    /**
    * CurrentCurrent: sensor
    * Current current flowing in/out of battery. Positive = Current flowing in to battery, e.g. during charging. Negative = Current flowing out of battery, e.g. during driving.
    *
    * Unit: A
    **/
    velocitas::DataPointFloat CurrentCurrent;

    /**
    * CurrentPower: sensor
    * Current electrical energy flowing in/out of battery. Positive = Energy flowing in to battery, e.g. during charging. Negative = Energy flowing out of battery, e.g. during driving.
    *
    * Unit: W
    **/
    velocitas::DataPointFloat CurrentPower;

    /**
    * CurrentVoltage: sensor
    * Current Voltage of the battery.
    *
    * Unit: V
    **/
    velocitas::DataPointFloat CurrentVoltage;

    /**
    * DCDC: branch
    * Properties related to DC/DC converter converting high voltage (from high voltage battery) to vehicle low voltage (supply voltage, typically 12 Volts).
    *
    **/
    vehicle::powertrain::traction_battery::dcdc::DCDC DCDC;

    /**
    * GrossCapacity: attribute (uint16)
    * Gross capacity of the battery.
    *
    * Unit: kWh
    **/
    velocitas::DataPointUint32 GrossCapacity;

    /**
    * Id: attribute (string)
    * Battery Identification Number as assigned by OEM.
    *
    * This could be serial number, part number plus serial number, UUID, or any other identifier that the OEM want to use to uniquely identify the battery individual.
    *
    **/
    velocitas::DataPointString Id;

    /**
    * IsGroundConnected: sensor
    * Indicating if the ground (negative terminator) of the traction battery is connected to the powertrain.
    *
    * It might be possible to disconnect the traction battery used by an electric powertrain. This is achieved by connectors, typically one for plus and one for minus.
    *
    **/
    velocitas::DataPointBoolean IsGroundConnected;

    /**
    * IsPowerConnected: sensor
    * Indicating if the power (positive terminator) of the traction battery is connected to the powertrain.
    *
    * It might be possible to disconnect the traction battery used by an electric powertrain. This is achieved by connectors, typically one for plus and one for minus.
    *
    **/
    velocitas::DataPointBoolean IsPowerConnected;

    /**
    * MaxVoltage: attribute (uint16)
    * Max allowed voltage of the battery, e.g. during charging.
    *
    * Unit: V
    **/
    velocitas::DataPointUint32 MaxVoltage;

    /**
    * NetCapacity: sensor
    * Total net capacity of the battery considering aging.
    *
    * Unit: kWh
    **/
    velocitas::DataPointUint32 NetCapacity;

    /**
    * NominalVoltage: attribute (uint16)
    * Nominal Voltage of the battery.
    *
    * Nominal voltage typically refers to voltage of fully charged battery when delivering rated capacity.
    *
    * Unit: V
    **/
    velocitas::DataPointUint32 NominalVoltage;

    /**
    * PowerLoss: sensor
    * Electrical energy lost by power dissipation to heat inside the battery.
    *
    * Unit: W
    **/
    velocitas::DataPointFloat PowerLoss;

    /**
    * ProductionDate: attribute (string)
    * Production date of battery in ISO8601 format, e.g. YYYY-MM-DD.
    *
    **/
    velocitas::DataPointString ProductionDate;

    /**
    * Range: sensor
    * Remaining range in meters using only battery.
    *
    * Unit: m
    **/
    velocitas::DataPointUint32 Range;

    /**
    * StateOfCharge: branch
    * Information on the state of charge of the vehicle's high voltage battery.
    *
    **/
    vehicle::powertrain::traction_battery::state_of_charge::StateOfCharge StateOfCharge;

    /**
    * StateOfHealth: sensor
    * Calculated battery state of health at standard conditions.
    *
    * Exact formula is implementation dependent. Could be e.g. current capacity at 20 degrees Celsius divided with original capacity at the same temperature.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat StateOfHealth;

    /**
    * Temperature: branch
    * Temperature Information for the battery pack.
    *
    **/
    vehicle::powertrain::traction_battery::temperature::Temperature Temperature;

};

} // namespace vehicle::powertrain::traction_battery

#endif // VEHICLE_POWERTRAIN_TRACTION_BATTERY_TRACTIONBATTERY_H
