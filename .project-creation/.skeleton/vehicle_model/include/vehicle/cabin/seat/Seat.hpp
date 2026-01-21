#ifndef VEHICLE_CABIN_SEAT_SEAT_H
#define VEHICLE_CABIN_SEAT_SEAT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/seat/row_1/Row1.hpp"
#include "vehicle/cabin/seat/row_2/Row2.hpp"

#include <string>

namespace vehicle::cabin::seat {
using ParentClass = velocitas::Model;

/** Seat model. */
class Seat : public ParentClass {
public:

    Seat(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Row1("Row1", this),
		Row2("Row2", this)
    {}

    /**
    * Row1: branch
    * All seats.
    *
    **/
    vehicle::cabin::seat::row_1::Row1 Row1;

    /**
    * Row2: branch
    * All seats.
    *
    **/
    vehicle::cabin::seat::row_2::Row2 Row2;

};

} // namespace vehicle::cabin::seat

#endif // VEHICLE_CABIN_SEAT_SEAT_H
