#ifndef VEHICLE_CABIN_INFOTAINMENT_NAVIGATION_NAVIGATION_H
#define VEHICLE_CABIN_INFOTAINMENT_NAVIGATION_NAVIGATION_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/infotainment/navigation/destination_set/DestinationSet.hpp"

#include <string>

namespace vehicle::cabin::infotainment::navigation {
using ParentClass = velocitas::Model;

/** Navigation model. */
class Navigation : public ParentClass {
public:

    Navigation(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		DestinationSet("DestinationSet", this)
    {}

    /**
    * DestinationSet: branch
    * A navigation has been selected.
    *
    **/
    vehicle::cabin::infotainment::navigation::destination_set::DestinationSet DestinationSet;

};

} // namespace vehicle::cabin::infotainment::navigation

#endif // VEHICLE_CABIN_INFOTAINMENT_NAVIGATION_NAVIGATION_H
