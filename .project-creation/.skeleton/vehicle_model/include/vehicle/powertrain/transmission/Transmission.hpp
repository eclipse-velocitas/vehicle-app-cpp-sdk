#ifndef VEHICLE_POWERTRAIN_TRANSMISSION_TRANSMISSION_H
#define VEHICLE_POWERTRAIN_TRANSMISSION_TRANSMISSION_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::powertrain::transmission {
using ParentClass = velocitas::Model;

/** Transmission model. */
class Transmission : public ParentClass {
public:

    Transmission(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		ClutchEngagement("ClutchEngagement", this),
		ClutchWear("ClutchWear", this),
		CurrentGear("CurrentGear", this),
		DiffLockFrontEngagement("DiffLockFrontEngagement", this),
		DiffLockRearEngagement("DiffLockRearEngagement", this),
		DriveType("DriveType", this),
		GearChangeMode("GearChangeMode", this),
		GearCount("GearCount", this),
		IsElectricalPowertrainEngaged("IsElectricalPowertrainEngaged", this),
		IsLowRangeEngaged("IsLowRangeEngaged", this),
		IsParkLockEngaged("IsParkLockEngaged", this),
		PerformanceMode("PerformanceMode", this),
		SelectedGear("SelectedGear", this),
		Temperature("Temperature", this),
		TorqueDistribution("TorqueDistribution", this),
		TravelledDistance("TravelledDistance", this),
		Type("Type", this)
    {}

    /**
    * ClutchEngagement: actuator
    * Clutch engagement. 0% = Clutch fully disengaged. 100% = Clutch fully engaged.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat ClutchEngagement;

    /**
    * ClutchWear: sensor
    * Clutch wear as a percent. 0 = no wear. 100 = worn.
    *
    * Value range: [, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 ClutchWear;

    /**
    * CurrentGear: sensor
    * The current gear. 0=Neutral, 1/2/..=Forward, -1/-2/..=Reverse.
    *
    **/
    velocitas::DataPointInt32 CurrentGear;

    /**
    * DiffLockFrontEngagement: actuator
    * Front Diff Lock engagement. 0% = Diff lock fully disengaged. 100% = Diff lock fully engaged.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat DiffLockFrontEngagement;

    /**
    * DiffLockRearEngagement: actuator
    * Rear Diff Lock engagement. 0% = Diff lock fully disengaged. 100% = Diff lock fully engaged.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat DiffLockRearEngagement;

    /**
    * DriveType: attribute (string)
    * Drive type.
    *
    * Allowed values: UNKNOWN, FORWARD_WHEEL_DRIVE, REAR_WHEEL_DRIVE, ALL_WHEEL_DRIVE
    **/
    velocitas::DataPointString DriveType;

    /**
    * GearChangeMode: actuator
    * Is the gearbox in automatic or manual (paddle) mode.
    *
    * Allowed values: MANUAL, AUTOMATIC
    **/
    velocitas::DataPointString GearChangeMode;

    /**
    * GearCount: attribute (int8)
    * Number of forward gears in the transmission. -1 = CVT.
    *
    **/
    velocitas::DataPointInt32 GearCount;

    /**
    * IsElectricalPowertrainEngaged: actuator
    * Is electrical powertrain mechanically connected/engaged to the drivetrain or not. False = Disconnected/Disengaged. True = Connected/Engaged.
    *
    * In some hybrid solutions it is possible to disconnect/disengage the electrical powertrain mechanically to avoid induced voltage reaching a too high level when driving at high speed.
    *
    **/
    velocitas::DataPointBoolean IsElectricalPowertrainEngaged;

    /**
    * IsLowRangeEngaged: actuator
    * Is gearbox in low range mode or not. False = Normal/High range engaged. True = Low range engaged.
    *
    * The possibility to switch between low and high gear range is typically only available in heavy vehicles and off-road vehicles.
    *
    **/
    velocitas::DataPointBoolean IsLowRangeEngaged;

    /**
    * IsParkLockEngaged: actuator
    * Is the transmission park lock engaged or not. False = Disengaged. True = Engaged.
    *
    **/
    velocitas::DataPointBoolean IsParkLockEngaged;

    /**
    * PerformanceMode: actuator
    * Current gearbox performance mode.
    *
    * Allowed values: NORMAL, SPORT, ECONOMY, SNOW, RAIN
    **/
    velocitas::DataPointString PerformanceMode;

    /**
    * SelectedGear: actuator
    * The selected gear. 0=Neutral, 1/2/..=Forward, -1/-2/..=Reverse, 126=Park, 127=Drive.
    *
    **/
    velocitas::DataPointInt32 SelectedGear;

    /**
    * Temperature: sensor
    * The current gearbox temperature.
    *
    * Unit: celsius
    **/
    velocitas::DataPointInt32 Temperature;

    /**
    * TorqueDistribution: actuator
    * Torque distribution between front and rear axle in percent. -100% = Full torque to front axle, 0% = 50:50 Front/Rear, 100% = Full torque to rear axle.
    *
    * Value range: [-100, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat TorqueDistribution;

    /**
    * TravelledDistance: sensor
    * Odometer reading, total distance travelled during the lifetime of the transmission.
    *
    * Unit: km
    **/
    velocitas::DataPointFloat TravelledDistance;

    /**
    * Type: attribute (string)
    * Transmission type.
    *
    * Allowed values: UNKNOWN, SEQUENTIAL, H, AUTOMATIC, DSG, CVT
    **/
    velocitas::DataPointString Type;

};

} // namespace vehicle::powertrain::transmission

#endif // VEHICLE_POWERTRAIN_TRANSMISSION_TRANSMISSION_H
