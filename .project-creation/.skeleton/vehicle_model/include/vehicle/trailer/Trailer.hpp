#ifndef VEHICLE_TRAILER_TRAILER_H
#define VEHICLE_TRAILER_TRAILER_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::trailer {
using ParentClass = velocitas::Model;

/** Trailer model. */
class Trailer : public ParentClass {
public:

    Trailer(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsConnected("IsConnected", this)
    {}

    /**
    * IsConnected: sensor
    * Signal indicating if trailer is connected or not.
    *
    **/
    velocitas::DataPointBoolean IsConnected;

};

} // namespace vehicle::trailer

#endif // VEHICLE_TRAILER_TRAILER_H
