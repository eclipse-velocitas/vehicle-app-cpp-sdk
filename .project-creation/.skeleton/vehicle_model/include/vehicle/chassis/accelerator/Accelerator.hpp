#ifndef VEHICLE_CHASSIS_ACCELERATOR_ACCELERATOR_H
#define VEHICLE_CHASSIS_ACCELERATOR_ACCELERATOR_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::chassis::accelerator {
using ParentClass = velocitas::Model;

/** Accelerator model. */
class Accelerator : public ParentClass {
public:

    Accelerator(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		PedalPosition("PedalPosition", this)
    {}

    /**
    * PedalPosition: sensor
    * Accelerator pedal position as percent. 0 = Not depressed. 100 = Fully depressed.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 PedalPosition;

};

} // namespace vehicle::chassis::accelerator

#endif // VEHICLE_CHASSIS_ACCELERATOR_ACCELERATOR_H
