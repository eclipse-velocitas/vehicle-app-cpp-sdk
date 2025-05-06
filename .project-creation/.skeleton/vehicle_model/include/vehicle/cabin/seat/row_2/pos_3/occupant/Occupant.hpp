#ifndef VEHICLE_CABIN_SEAT_ROW_2_POS_3_OCCUPANT_OCCUPANT_H
#define VEHICLE_CABIN_SEAT_ROW_2_POS_3_OCCUPANT_OCCUPANT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/seat/row_2/pos_3/occupant/identifier/Identifier.hpp"

#include <string>

namespace vehicle::cabin::seat::row_2::pos_3::occupant {
using ParentClass = velocitas::Model;

/** Occupant model. */
class Occupant : public ParentClass {
public:

    Occupant(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Identifier("Identifier", this)
    {}

    /**
    * Identifier: branch
    * Identifier attributes based on OAuth 2.0.
    *
    **/
    vehicle::cabin::seat::row_2::pos_3::occupant::identifier::Identifier Identifier;

};

} // namespace vehicle::cabin::seat::row_2::pos_3::occupant

#endif // VEHICLE_CABIN_SEAT_ROW_2_POS_3_OCCUPANT_OCCUPANT_H
