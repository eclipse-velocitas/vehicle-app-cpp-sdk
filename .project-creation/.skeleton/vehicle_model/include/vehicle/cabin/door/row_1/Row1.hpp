#ifndef VEHICLE_CABIN_DOOR_ROW_1_ROW1_H
#define VEHICLE_CABIN_DOOR_ROW_1_ROW1_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/door/row_1/left/Left.hpp"
#include "vehicle/cabin/door/row_1/right/Right.hpp"

#include <string>

namespace vehicle::cabin::door::row_1 {
using ParentClass = velocitas::Model;

/** Row1 model. */
class Row1 : public ParentClass {
public:

    Row1(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Left("Left", this),
		Right("Right", this)
    {}

    /**
    * Left: branch
    * All doors, including windows and switches.
    *
    **/
    vehicle::cabin::door::row_1::left::Left Left;

    /**
    * Right: branch
    * All doors, including windows and switches.
    *
    **/
    vehicle::cabin::door::row_1::right::Right Right;

};

} // namespace vehicle::cabin::door::row_1

#endif // VEHICLE_CABIN_DOOR_ROW_1_ROW1_H
