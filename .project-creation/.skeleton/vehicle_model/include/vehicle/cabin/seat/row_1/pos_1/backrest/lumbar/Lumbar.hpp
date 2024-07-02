#ifndef VEHICLE_CABIN_SEAT_ROW_1_POS_1_BACKREST_LUMBAR_LUMBAR_H
#define VEHICLE_CABIN_SEAT_ROW_1_POS_1_BACKREST_LUMBAR_LUMBAR_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::seat::row_1::pos_1::backrest::lumbar {
using ParentClass = velocitas::Model;

/** Lumbar model. */
class Lumbar : public ParentClass {
public:

    Lumbar(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Height("Height", this),
		Support("Support", this)
    {}

    /**
    * Height: actuator
    * Height of lumbar support. Position is relative within available movable range of the lumbar support. 0 = Lowermost position supported.
    *
    * Value range: [0, ]
    * Unit: mm
    **/
    velocitas::DataPointUint32 Height;

    /**
    * Support: actuator
    * Lumbar support (in/out position). 0 = Innermost position. 100 = Outermost position.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat Support;

};

} // namespace vehicle::cabin::seat::row_1::pos_1::backrest::lumbar

#endif // VEHICLE_CABIN_SEAT_ROW_1_POS_1_BACKREST_LUMBAR_LUMBAR_H
