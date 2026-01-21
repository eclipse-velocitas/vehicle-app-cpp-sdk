#ifndef VEHICLE_CABIN_SEAT_ROW_1_ROW1_H
#define VEHICLE_CABIN_SEAT_ROW_1_ROW1_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/seat/row_1/pos_1/Pos1.hpp"
#include "vehicle/cabin/seat/row_1/pos_2/Pos2.hpp"
#include "vehicle/cabin/seat/row_1/pos_3/Pos3.hpp"

#include <string>

namespace vehicle::cabin::seat::row_1 {
using ParentClass = velocitas::Model;

/** Row1 model. */
class Row1 : public ParentClass {
public:

    Row1(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Pos1("Pos1", this),
		Pos2("Pos2", this),
		Pos3("Pos3", this)
    {}

    /**
    * Pos1: branch
    * All seats.
    *
    **/
    vehicle::cabin::seat::row_1::pos_1::Pos1 Pos1;

    /**
    * Pos2: branch
    * All seats.
    *
    **/
    vehicle::cabin::seat::row_1::pos_2::Pos2 Pos2;

    /**
    * Pos3: branch
    * All seats.
    *
    **/
    vehicle::cabin::seat::row_1::pos_3::Pos3 Pos3;

};

} // namespace vehicle::cabin::seat::row_1

#endif // VEHICLE_CABIN_SEAT_ROW_1_ROW1_H
