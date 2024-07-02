#ifndef VEHICLE_CABIN_DOOR_DOOR_H
#define VEHICLE_CABIN_DOOR_DOOR_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/door/row_1/Row1.hpp"
#include "vehicle/cabin/door/row_2/Row2.hpp"

#include <string>

namespace vehicle::cabin::door {
using ParentClass = velocitas::Model;

/** Door model. */
class Door : public ParentClass {
public:

    Door(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Row1("Row1", this),
		Row2("Row2", this)
    {}

    /**
    * Row1: branch
    * All doors, including windows and switches.
    *
    **/
    vehicle::cabin::door::row_1::Row1 Row1;

    /**
    * Row2: branch
    * All doors, including windows and switches.
    *
    **/
    vehicle::cabin::door::row_2::Row2 Row2;

};

} // namespace vehicle::cabin::door

#endif // VEHICLE_CABIN_DOOR_DOOR_H
