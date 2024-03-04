#ifndef VEHICLE_OBD_STATUS_STATUS_H
#define VEHICLE_OBD_STATUS_STATUS_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::obd::status {
using ParentClass = velocitas::Model;

/** Status model. */
class Status : public ParentClass {
public:

    Status(const std::string& name, ParentClass* parent) :
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
    * Malfunction Indicator Light (MIL) False = Off, True = On
    *
    **/
    velocitas::DataPointBoolean IsMILOn;

};

} // namespace vehicle::obd::status

#endif // VEHICLE_OBD_STATUS_STATUS_H
