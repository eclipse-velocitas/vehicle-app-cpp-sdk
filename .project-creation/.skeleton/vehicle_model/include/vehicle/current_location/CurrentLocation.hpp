#ifndef VEHICLE_CURRENT_LOCATION_CURRENTLOCATION_H
#define VEHICLE_CURRENT_LOCATION_CURRENTLOCATION_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/current_location/gnss_receiver/GNSSReceiver.hpp"

#include <string>

namespace vehicle::current_location {
using ParentClass = velocitas::Model;

/** CurrentLocation model. */
class CurrentLocation : public ParentClass {
public:

    CurrentLocation(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Altitude("Altitude", this),
		GNSSReceiver("GNSSReceiver", this),
		Heading("Heading", this),
		HorizontalAccuracy("HorizontalAccuracy", this),
		Latitude("Latitude", this),
		Longitude("Longitude", this),
		Timestamp("Timestamp", this),
		VerticalAccuracy("VerticalAccuracy", this)
    {}

    /**
    * Altitude: sensor
    * Current altitude relative to WGS 84 reference ellipsoid, as measured at the position of GNSS receiver antenna.
    *
    * Unit: m
    **/
    velocitas::DataPointDouble Altitude;

    /**
    * GNSSReceiver: branch
    * Information on the GNSS receiver used for determining current location.
    *
    **/
    vehicle::current_location::gnss_receiver::GNSSReceiver GNSSReceiver;

    /**
    * Heading: sensor
    * Current heading relative to geographic north. 0 = North, 90 = East, 180 = South, 270 = West.
    *
    * Value range: [0, 360]
    * Unit: degrees
    **/
    velocitas::DataPointDouble Heading;

    /**
    * HorizontalAccuracy: sensor
    * Accuracy of the latitude and longitude coordinates.
    *
    * Unit: m
    **/
    velocitas::DataPointDouble HorizontalAccuracy;

    /**
    * Latitude: sensor
    * Current latitude of vehicle in WGS 84 geodetic coordinates, as measured at the position of GNSS receiver antenna.
    *
    * Value range: [-90, 90]
    * Unit: degrees
    **/
    velocitas::DataPointDouble Latitude;

    /**
    * Longitude: sensor
    * Current longitude of vehicle in WGS 84 geodetic coordinates, as measured at the position of GNSS receiver antenna.
    *
    * Value range: [-180, 180]
    * Unit: degrees
    **/
    velocitas::DataPointDouble Longitude;

    /**
    * Timestamp: sensor
    * Timestamp from GNSS system for current location, formatted according to ISO 8601 with UTC time zone.
    *
    **/
    velocitas::DataPointString Timestamp;

    /**
    * VerticalAccuracy: sensor
    * Accuracy of altitude.
    *
    * Unit: m
    **/
    velocitas::DataPointDouble VerticalAccuracy;

};

} // namespace vehicle::current_location

#endif // VEHICLE_CURRENT_LOCATION_CURRENTLOCATION_H
