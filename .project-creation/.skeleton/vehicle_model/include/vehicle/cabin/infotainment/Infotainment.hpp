#ifndef VEHICLE_CABIN_INFOTAINMENT_INFOTAINMENT_H
#define VEHICLE_CABIN_INFOTAINMENT_INFOTAINMENT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/infotainment/hmi/HMI.hpp"
#include "vehicle/cabin/infotainment/media/Media.hpp"
#include "vehicle/cabin/infotainment/navigation/Navigation.hpp"

#include <string>

namespace vehicle::cabin::infotainment {
using ParentClass = velocitas::Model;

/** Infotainment model. */
class Infotainment : public ParentClass {
public:

    Infotainment(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		HMI("HMI", this),
		Media("Media", this),
		Navigation("Navigation", this)
    {}

    /**
    * HMI: branch
    * HMI related signals
    *
    **/
    vehicle::cabin::infotainment::hmi::HMI HMI;

    /**
    * Media: branch
    * All Media actions
    *
    **/
    vehicle::cabin::infotainment::media::Media Media;

    /**
    * Navigation: branch
    * All navigation actions
    *
    **/
    vehicle::cabin::infotainment::navigation::Navigation Navigation;

};

} // namespace vehicle::cabin::infotainment

#endif // VEHICLE_CABIN_INFOTAINMENT_INFOTAINMENT_H
