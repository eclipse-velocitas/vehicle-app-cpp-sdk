#ifndef VEHICLE_CABIN_INFOTAINMENT_NAVIGATION_DESTINATION_SET_DESTINATIONSET_H
#define VEHICLE_CABIN_INFOTAINMENT_NAVIGATION_DESTINATION_SET_DESTINATIONSET_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::infotainment::navigation::destination_set {
using ParentClass = velocitas::Model;

/** DestinationSet model. */
class DestinationSet : public ParentClass {
public:

    DestinationSet(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Latitude("Latitude", this),
		Longitude("Longitude", this)
    {}

    /**
    * Latitude: actuator
    * Latitude of destination in WGS 84 geodetic coordinates.
    *
    * Value range: [-90, 90]
    * Unit: degrees
    **/
    velocitas::DataPointDouble Latitude;

    /**
    * Longitude: actuator
    * Longitude of destination in WGS 84 geodetic coordinates.
    *
    * Value range: [-180, 180]
    * Unit: degrees
    **/
    velocitas::DataPointDouble Longitude;

};

} // namespace vehicle::cabin::infotainment::navigation::destination_set

#endif // VEHICLE_CABIN_INFOTAINMENT_NAVIGATION_DESTINATION_SET_DESTINATIONSET_H
