#ifndef VEHICLE_CABIN_CONVERTIBLE_CONVERTIBLE_H
#define VEHICLE_CABIN_CONVERTIBLE_CONVERTIBLE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::convertible {
using ParentClass = velocitas::Model;

/** Convertible model. */
class Convertible : public ParentClass {
public:

    Convertible(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Status("Status", this)
    {}

    /**
    * Status: sensor
    * Roof status on convertible vehicles.
    *
    * Allowed values: UNDEFINED, CLOSED, OPEN, CLOSING, OPENING, STALLED
    **/
    velocitas::DataPointString Status;

};

} // namespace vehicle::cabin::convertible

#endif // VEHICLE_CABIN_CONVERTIBLE_CONVERTIBLE_H
