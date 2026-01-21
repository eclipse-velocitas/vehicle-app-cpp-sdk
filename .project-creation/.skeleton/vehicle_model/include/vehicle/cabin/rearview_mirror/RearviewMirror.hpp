#ifndef VEHICLE_CABIN_REARVIEW_MIRROR_REARVIEWMIRROR_H
#define VEHICLE_CABIN_REARVIEW_MIRROR_REARVIEWMIRROR_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::rearview_mirror {
using ParentClass = velocitas::Model;

/** RearviewMirror model. */
class RearviewMirror : public ParentClass {
public:

    RearviewMirror(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		DimmingLevel("DimmingLevel", this)
    {}

    /**
    * DimmingLevel: actuator
    * Dimming level of rearview mirror. 0 = undimmed. 100 = fully dimmed.
    *
    * Value range: [, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 DimmingLevel;

};

} // namespace vehicle::cabin::rearview_mirror

#endif // VEHICLE_CABIN_REARVIEW_MIRROR_REARVIEWMIRROR_H
