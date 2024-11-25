#ifndef VEHICLE_BODY_RAINDETECTION_RAINDETECTION_H
#define VEHICLE_BODY_RAINDETECTION_RAINDETECTION_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::body::raindetection {
using ParentClass = velocitas::Model;

/** Raindetection model. */
class Raindetection : public ParentClass {
public:

    Raindetection(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Intensity("Intensity", this)
    {}

    /**
    * Intensity: sensor
    * Rain intensity. 0 = Dry, No Rain. 100 = Covered.
    *
    * Value range: [, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 Intensity;

};

} // namespace vehicle::body::raindetection

#endif // VEHICLE_BODY_RAINDETECTION_RAINDETECTION_H
