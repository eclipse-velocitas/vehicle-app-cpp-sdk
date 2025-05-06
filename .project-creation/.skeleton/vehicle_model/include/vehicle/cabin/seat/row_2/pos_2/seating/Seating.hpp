#ifndef VEHICLE_CABIN_SEAT_ROW_2_POS_2_SEATING_SEATING_H
#define VEHICLE_CABIN_SEAT_ROW_2_POS_2_SEATING_SEATING_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::seat::row_2::pos_2::seating {
using ParentClass = velocitas::Model;

/** Seating model. */
class Seating : public ParentClass {
public:

    Seating(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Length("Length", this)
    {}

    /**
    * Length: actuator
    * Length adjustment of seating. 0 = Adjustable part of seating in rearmost position (Shortest length of seating).
    *
    * Value range: [0, ]
    * Unit: mm
    **/
    velocitas::DataPointUint32 Length;

};

} // namespace vehicle::cabin::seat::row_2::pos_2::seating

#endif // VEHICLE_CABIN_SEAT_ROW_2_POS_2_SEATING_SEATING_H
