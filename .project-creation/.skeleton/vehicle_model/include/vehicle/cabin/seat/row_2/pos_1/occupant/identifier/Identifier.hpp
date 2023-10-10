#ifndef VEHICLE_CABIN_SEAT_ROW_2_POS_1_OCCUPANT_IDENTIFIER_IDENTIFIER_H
#define VEHICLE_CABIN_SEAT_ROW_2_POS_1_OCCUPANT_IDENTIFIER_IDENTIFIER_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::seat::row_2::pos_1::occupant::identifier {
using ParentClass = velocitas::Model;

/** Identifier model. */
class Identifier : public ParentClass {
public:

    Identifier(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Issuer("Issuer", this),
		Subject("Subject", this)
    {}

    /**
    * Issuer: sensor
    * Unique Issuer for the authentication of the occupant. E.g. https://accounts.funcorp.com.
    *
    **/
    velocitas::DataPointString Issuer;

    /**
    * Subject: sensor
    * Subject for the authentication of the occupant. E.g. UserID 7331677.
    *
    **/
    velocitas::DataPointString Subject;

};

} // namespace vehicle::cabin::seat::row_2::pos_1::occupant::identifier

#endif // VEHICLE_CABIN_SEAT_ROW_2_POS_1_OCCUPANT_IDENTIFIER_IDENTIFIER_H
