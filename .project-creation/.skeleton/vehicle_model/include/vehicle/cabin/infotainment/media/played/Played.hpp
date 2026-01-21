#ifndef VEHICLE_CABIN_INFOTAINMENT_MEDIA_PLAYED_PLAYED_H
#define VEHICLE_CABIN_INFOTAINMENT_MEDIA_PLAYED_PLAYED_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::infotainment::media::played {
using ParentClass = velocitas::Model;

/** Played model. */
class Played : public ParentClass {
public:

    Played(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Album("Album", this),
		Artist("Artist", this),
		Source("Source", this),
		Track("Track", this),
		URI("URI", this)
    {}

    /**
    * Album: sensor
    * Name of album being played
    *
    **/
    velocitas::DataPointString Album;

    /**
    * Artist: sensor
    * Name of artist being played
    *
    **/
    velocitas::DataPointString Artist;

    /**
    * Source: actuator
    * Media selected for playback
    *
    * Allowed values: UNKNOWN, SIRIUS_XM, AM, FM, DAB, TV, CD, DVD, AUX, USB, DISK, BLUETOOTH, INTERNET, VOICE, BEEP
    **/
    velocitas::DataPointString Source;

    /**
    * Track: sensor
    * Name of track being played
    *
    **/
    velocitas::DataPointString Track;

    /**
    * URI: sensor
    * User Resource associated with the media
    *
    **/
    velocitas::DataPointString URI;

};

} // namespace vehicle::cabin::infotainment::media::played

#endif // VEHICLE_CABIN_INFOTAINMENT_MEDIA_PLAYED_PLAYED_H
