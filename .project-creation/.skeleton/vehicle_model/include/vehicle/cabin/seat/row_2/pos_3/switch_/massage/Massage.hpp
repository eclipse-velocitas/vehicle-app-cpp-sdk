#ifndef VEHICLE_CABIN_SEAT_ROW_2_POS_3_SWITCH__MASSAGE_MASSAGE_H
#define VEHICLE_CABIN_SEAT_ROW_2_POS_3_SWITCH__MASSAGE_MASSAGE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::seat::row_2::pos_3::switch_::massage {
using ParentClass = velocitas::Model;

/** Massage model. */
class Massage : public ParentClass {
public:

    Massage(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsDecreaseEngaged("IsDecreaseEngaged", this),
		IsIncreaseEngaged("IsIncreaseEngaged", this)
    {}

    /**
    * IsDecreaseEngaged: actuator
    * Decrease massage level switch engaged (SingleSeat.Massage).
    *
    **/
    velocitas::DataPointBoolean IsDecreaseEngaged;

    /**
    * IsIncreaseEngaged: actuator
    * Increase massage level switch engaged (SingleSeat.Massage).
    *
    **/
    velocitas::DataPointBoolean IsIncreaseEngaged;

};

} // namespace vehicle::cabin::seat::row_2::pos_3::switch_::massage

#endif // VEHICLE_CABIN_SEAT_ROW_2_POS_3_SWITCH__MASSAGE_MASSAGE_H
