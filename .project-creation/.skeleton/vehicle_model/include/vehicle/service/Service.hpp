#ifndef VEHICLE_SERVICE_SERVICE_H
#define VEHICLE_SERVICE_SERVICE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::service {
using ParentClass = velocitas::Model;

/** Service model. */
class Service : public ParentClass {
public:

    Service(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		DistanceToService("DistanceToService", this),
		IsServiceDue("IsServiceDue", this),
		TimeToService("TimeToService", this)
    {}

    /**
    * DistanceToService: sensor
    * Remaining distance to service (of any kind). Negative values indicate service overdue.
    *
    * Unit: km
    **/
    velocitas::DataPointFloat DistanceToService;

    /**
    * IsServiceDue: sensor
    * Indicates if vehicle needs service (of any kind). True = Service needed now or in the near future. False = No known need for service.
    *
    **/
    velocitas::DataPointBoolean IsServiceDue;

    /**
    * TimeToService: sensor
    * Remaining time to service (of any kind). Negative values indicate service overdue.
    *
    * Unit: s
    **/
    velocitas::DataPointInt32 TimeToService;

};

} // namespace vehicle::service

#endif // VEHICLE_SERVICE_SERVICE_H
