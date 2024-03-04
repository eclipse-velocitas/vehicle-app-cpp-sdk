#ifndef VEHICLE_ADAS_ESC_ROAD_FRICTION_ROADFRICTION_H
#define VEHICLE_ADAS_ESC_ROAD_FRICTION_ROADFRICTION_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::adas::esc::road_friction {
using ParentClass = velocitas::Model;

/** RoadFriction model. */
class RoadFriction : public ParentClass {
public:

    RoadFriction(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		LowerBound("LowerBound", this),
		MostProbable("MostProbable", this),
		UpperBound("UpperBound", this)
    {}

    /**
    * LowerBound: sensor
    * Lower bound road friction, as calculated by the ESC system. 5% possibility that road friction is below this value. 0 = no friction, 100 = maximum friction.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat LowerBound;

    /**
    * MostProbable: sensor
    * Most probable road friction, as calculated by the ESC system. Exact meaning of most probable is implementation specific. 0 = no friction, 100 = maximum friction.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat MostProbable;

    /**
    * UpperBound: sensor
    * Upper bound road friction, as calculated by the ESC system. 95% possibility that road friction is below this value. 0 = no friction, 100 = maximum friction.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointFloat UpperBound;

};

} // namespace vehicle::adas::esc::road_friction

#endif // VEHICLE_ADAS_ESC_ROAD_FRICTION_ROADFRICTION_H
