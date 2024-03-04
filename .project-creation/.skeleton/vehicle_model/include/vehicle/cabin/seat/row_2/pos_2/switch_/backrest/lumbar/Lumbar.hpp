#ifndef VEHICLE_CABIN_SEAT_ROW_2_POS_2_SWITCH__BACKREST_LUMBAR_LUMBAR_H
#define VEHICLE_CABIN_SEAT_ROW_2_POS_2_SWITCH__BACKREST_LUMBAR_LUMBAR_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::seat::row_2::pos_2::switch_::backrest::lumbar {
using ParentClass = velocitas::Model;

/** Lumbar model. */
class Lumbar : public ParentClass {
public:

    Lumbar(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsDownEngaged("IsDownEngaged", this),
		IsLessSupportEngaged("IsLessSupportEngaged", this),
		IsMoreSupportEngaged("IsMoreSupportEngaged", this),
		IsUpEngaged("IsUpEngaged", this)
    {}

    /**
    * IsDownEngaged: actuator
    * Lumbar down switch engaged (SingleSeat.Backrest.Lumbar.Support).
    *
    **/
    velocitas::DataPointBoolean IsDownEngaged;

    /**
    * IsLessSupportEngaged: actuator
    * Is switch for less lumbar support engaged (SingleSeat.Backrest.Lumbar.Support).
    *
    **/
    velocitas::DataPointBoolean IsLessSupportEngaged;

    /**
    * IsMoreSupportEngaged: actuator
    * Is switch for more lumbar support engaged (SingleSeat.Backrest.Lumbar.Support).
    *
    **/
    velocitas::DataPointBoolean IsMoreSupportEngaged;

    /**
    * IsUpEngaged: actuator
    * Lumbar up switch engaged (SingleSeat.Backrest.Lumbar.Support).
    *
    **/
    velocitas::DataPointBoolean IsUpEngaged;

};

} // namespace vehicle::cabin::seat::row_2::pos_2::switch_::backrest::lumbar

#endif // VEHICLE_CABIN_SEAT_ROW_2_POS_2_SWITCH__BACKREST_LUMBAR_LUMBAR_H
