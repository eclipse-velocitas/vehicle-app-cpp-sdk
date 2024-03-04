#ifndef VEHICLE_CABIN_SEAT_ROW_1_POS_2_BACKREST_SIDE_BOLSTER_SIDEBOLSTER_H
#define VEHICLE_CABIN_SEAT_ROW_1_POS_2_BACKREST_SIDE_BOLSTER_SIDEBOLSTER_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::seat::row_1::pos_2::backrest::side_bolster {
using ParentClass = velocitas::Model;

/** SideBolster model. */
class SideBolster : public ParentClass {
public:

    SideBolster(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Support("Support", this)
    {}

    /**
    * Support: actuator
    * Side bolster support. 0 = Minimum support (widest side bolster setting). 100 = Maximum support.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat Support;

};

} // namespace vehicle::cabin::seat::row_1::pos_2::backrest::side_bolster

#endif // VEHICLE_CABIN_SEAT_ROW_1_POS_2_BACKREST_SIDE_BOLSTER_SIDEBOLSTER_H
