#ifndef VEHICLE_CABIN_INFOTAINMENT_MEDIA_MEDIA_H
#define VEHICLE_CABIN_INFOTAINMENT_MEDIA_MEDIA_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/infotainment/media/played/Played.hpp"

#include <string>

namespace vehicle::cabin::infotainment::media {
using ParentClass = velocitas::Model;

/** Media model. */
class Media : public ParentClass {
public:

    Media(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Action("Action", this),
		DeclinedURI("DeclinedURI", this),
		Played("Played", this),
		SelectedURI("SelectedURI", this),
		Volume("Volume", this)
    {}

    /**
    * Action: actuator
    * Tells if the media was
    *
    * Allowed values: UNKNOWN, STOP, PLAY, FAST_FORWARD, FAST_BACKWARD, SKIP_FORWARD, SKIP_BACKWARD
    **/
    velocitas::DataPointString Action;

    /**
    * DeclinedURI: sensor
    * URI of suggested media that was declined
    *
    **/
    velocitas::DataPointString DeclinedURI;

    /**
    * Played: branch
    * Collection of signals updated in concert when a new media is played
    *
    **/
    vehicle::cabin::infotainment::media::played::Played Played;

    /**
    * SelectedURI: actuator
    * URI of suggested media that was selected
    *
    **/
    velocitas::DataPointString SelectedURI;

    /**
    * Volume: actuator
    * Current Media Volume
    *
    * Value range: [0, 100]
    **/
    velocitas::DataPointUint32 Volume;

};

} // namespace vehicle::cabin::infotainment::media

#endif // VEHICLE_CABIN_INFOTAINMENT_MEDIA_MEDIA_H
