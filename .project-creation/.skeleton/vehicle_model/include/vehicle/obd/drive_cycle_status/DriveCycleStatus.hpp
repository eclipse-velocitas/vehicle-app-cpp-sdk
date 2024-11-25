#ifndef VEHICLE_OBD_DRIVE_CYCLE_STATUS_DRIVECYCLESTATUS_H
#define VEHICLE_OBD_DRIVE_CYCLE_STATUS_DRIVECYCLESTATUS_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::obd::drive_cycle_status {
using ParentClass = velocitas::Model;

/** DriveCycleStatus model. */
class DriveCycleStatus : public ParentClass {
public:

    DriveCycleStatus(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		DTCCount("DTCCount", this),
		IgnitionType("IgnitionType", this),
		IsMILOn("IsMILOn", this)
    {}

    /**
    * DTCCount: sensor
    * Number of sensor Trouble Codes (DTC)
    *
    **/
    velocitas::DataPointUint32 DTCCount;

    /**
    * IgnitionType: sensor
    * Type of the ignition for ICE - spark = spark plug ignition, compression = self-igniting (Diesel engines)
    *
    * Allowed values: SPARK, COMPRESSION
    **/
    velocitas::DataPointString IgnitionType;

    /**
    * IsMILOn: sensor
    * Malfunction Indicator Light (MIL) - False = Off, True = On
    *
    **/
    velocitas::DataPointBoolean IsMILOn;

};

} // namespace vehicle::obd::drive_cycle_status

#endif // VEHICLE_OBD_DRIVE_CYCLE_STATUS_DRIVECYCLESTATUS_H
