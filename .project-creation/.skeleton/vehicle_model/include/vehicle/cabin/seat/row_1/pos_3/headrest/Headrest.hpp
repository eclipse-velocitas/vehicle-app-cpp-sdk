#ifndef VEHICLE_CABIN_SEAT_ROW_1_POS_3_HEADREST_HEADREST_H
#define VEHICLE_CABIN_SEAT_ROW_1_POS_3_HEADREST_HEADREST_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::seat::row_1::pos_3::headrest {
using ParentClass = velocitas::Model;

/** Headrest model. */
class Headrest : public ParentClass {
public:

    Headrest(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Angle("Angle", this),
		Height("Height", this)
    {}

    /**
    * Angle: actuator
    * Headrest angle, relative to backrest, 0 degrees if parallel to backrest, Positive degrees = tilted forward.
    *
    * Unit: degrees
    **/
    velocitas::DataPointFloat Angle;

    /**
    * Height: actuator
    * Position of headrest relative to movable range of the head rest. 0 = Bottommost position supported.
    *
    * Value range: [0, ]
    * Unit: mm
    **/
    velocitas::DataPointUint32 Height;

};

} // namespace vehicle::cabin::seat::row_1::pos_3::headrest

#endif // VEHICLE_CABIN_SEAT_ROW_1_POS_3_HEADREST_HEADREST_H
