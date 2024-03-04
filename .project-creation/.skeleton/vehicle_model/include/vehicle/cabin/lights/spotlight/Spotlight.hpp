#ifndef VEHICLE_CABIN_LIGHTS_SPOTLIGHT_SPOTLIGHT_H
#define VEHICLE_CABIN_LIGHTS_SPOTLIGHT_SPOTLIGHT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/lights/spotlight/row_1/Row1.hpp"
#include "vehicle/cabin/lights/spotlight/row_2/Row2.hpp"
#include "vehicle/cabin/lights/spotlight/row_3/Row3.hpp"
#include "vehicle/cabin/lights/spotlight/row_4/Row4.hpp"

#include <string>

namespace vehicle::cabin::lights::spotlight {
using ParentClass = velocitas::Model;

/** Spotlight model. */
class Spotlight : public ParentClass {
public:

    Spotlight(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Row1("Row1", this),
		Row2("Row2", this),
		Row3("Row3", this),
		Row4("Row4", this)
    {}

    /**
    * Row1: branch
    * Spotlight for a specific area in the vehicle.
    *
    **/
    vehicle::cabin::lights::spotlight::row_1::Row1 Row1;

    /**
    * Row2: branch
    * Spotlight for a specific area in the vehicle.
    *
    **/
    vehicle::cabin::lights::spotlight::row_2::Row2 Row2;

    /**
    * Row3: branch
    * Spotlight for a specific area in the vehicle.
    *
    **/
    vehicle::cabin::lights::spotlight::row_3::Row3 Row3;

    /**
    * Row4: branch
    * Spotlight for a specific area in the vehicle.
    *
    **/
    vehicle::cabin::lights::spotlight::row_4::Row4 Row4;

};

} // namespace vehicle::cabin::lights::spotlight

#endif // VEHICLE_CABIN_LIGHTS_SPOTLIGHT_SPOTLIGHT_H
