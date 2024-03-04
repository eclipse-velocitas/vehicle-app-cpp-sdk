#ifndef VEHICLE_CONNECTIVITY_CONNECTIVITY_H
#define VEHICLE_CONNECTIVITY_CONNECTIVITY_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::connectivity {
using ParentClass = velocitas::Model;

/** Connectivity model. */
class Connectivity : public ParentClass {
public:

    Connectivity(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsConnectivityAvailable("IsConnectivityAvailable", this)
    {}

    /**
    * IsConnectivityAvailable: sensor
    * Indicates if connectivity between vehicle and cloud is available. True = Connectivity is available. False = Connectivity is not available.
    *
    * This signal can be used by onboard vehicle services to decide what features that shall be offered to the driver, for example disable the 'check for update' button if vehicle does not have connectivity.
    *
    **/
    velocitas::DataPointBoolean IsConnectivityAvailable;

};

} // namespace vehicle::connectivity

#endif // VEHICLE_CONNECTIVITY_CONNECTIVITY_H
