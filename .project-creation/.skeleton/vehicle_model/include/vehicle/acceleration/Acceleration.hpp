#ifndef VEHICLE_ACCELERATION_ACCELERATION_H
#define VEHICLE_ACCELERATION_ACCELERATION_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::acceleration {
using ParentClass = velocitas::Model;

/** Acceleration model. */
class Acceleration : public ParentClass {
public:

    Acceleration(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Lateral("Lateral", this),
		Longitudinal("Longitudinal", this),
		Vertical("Vertical", this)
    {}

    /**
    * Lateral: sensor
    * Vehicle acceleration in Y (lateral acceleration).
    *
    * Unit: m/s^2
    **/
    velocitas::DataPointFloat Lateral;

    /**
    * Longitudinal: sensor
    * Vehicle acceleration in X (longitudinal acceleration).
    *
    * Unit: m/s^2
    **/
    velocitas::DataPointFloat Longitudinal;

    /**
    * Vertical: sensor
    * Vehicle acceleration in Z (vertical acceleration).
    *
    * Unit: m/s^2
    **/
    velocitas::DataPointFloat Vertical;

};

} // namespace vehicle::acceleration

#endif // VEHICLE_ACCELERATION_ACCELERATION_H
