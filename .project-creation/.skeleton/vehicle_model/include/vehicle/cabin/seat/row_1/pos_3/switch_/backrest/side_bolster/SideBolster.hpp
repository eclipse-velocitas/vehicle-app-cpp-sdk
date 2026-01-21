#ifndef VEHICLE_CABIN_SEAT_ROW_1_POS_3_SWITCH__BACKREST_SIDE_BOLSTER_SIDEBOLSTER_H
#define VEHICLE_CABIN_SEAT_ROW_1_POS_3_SWITCH__BACKREST_SIDE_BOLSTER_SIDEBOLSTER_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::seat::row_1::pos_3::switch_::backrest::side_bolster {
using ParentClass = velocitas::Model;

/** SideBolster model. */
class SideBolster : public ParentClass {
public:

    SideBolster(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsLessSupportEngaged("IsLessSupportEngaged", this),
		IsMoreSupportEngaged("IsMoreSupportEngaged", this)
    {}

    /**
    * IsLessSupportEngaged: actuator
    * Is switch for less side bolster support engaged (SingleSeat.Backrest.SideBolster.Support).
    *
    **/
    velocitas::DataPointBoolean IsLessSupportEngaged;

    /**
    * IsMoreSupportEngaged: actuator
    * Is switch for more side bolster support engaged (SingleSeat.Backrest.SideBolster.Support).
    *
    **/
    velocitas::DataPointBoolean IsMoreSupportEngaged;

};

} // namespace vehicle::cabin::seat::row_1::pos_3::switch_::backrest::side_bolster

#endif // VEHICLE_CABIN_SEAT_ROW_1_POS_3_SWITCH__BACKREST_SIDE_BOLSTER_SIDEBOLSTER_H
