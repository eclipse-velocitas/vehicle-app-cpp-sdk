#ifndef VEHICLE_CURRENT_LOCATION_GNSS_RECEIVER_MOUNTING_POSITION_MOUNTINGPOSITION_H
#define VEHICLE_CURRENT_LOCATION_GNSS_RECEIVER_MOUNTING_POSITION_MOUNTINGPOSITION_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::current_location::gnss_receiver::mounting_position {
using ParentClass = velocitas::Model;

/** MountingPosition model. */
class MountingPosition : public ParentClass {
public:

    MountingPosition(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		X("X", this),
		Y("Y", this),
		Z("Z", this)
    {}

    /**
    * X: attribute (int16)
    * Mounting position of GNSS receiver antenna relative to vehicle coordinate system. Axis definitions according to ISO 8855. Origin at center of (first) rear axle. Positive values = forward of rear axle. Negative values = backward of rear axle.
    *
    * Unit: mm
    **/
    velocitas::DataPointInt32 X;

    /**
    * Y: attribute (int16)
    * Mounting position of GNSS receiver antenna relative to vehicle coordinate system. Axis definitions according to ISO 8855. Origin at center of (first) rear axle. Positive values = left of origin. Negative values = right of origin. Left/Right is as seen from driver perspective, i.e. by a person looking forward.
    *
    * Unit: mm
    **/
    velocitas::DataPointInt32 Y;

    /**
    * Z: attribute (int16)
    * Mounting position of GNSS receiver on Z-axis. Axis definitions according to ISO 8855. Origin at center of (first) rear axle. Positive values = above center of rear axle. Negative values = below center of rear axle.
    *
    * Unit: mm
    **/
    velocitas::DataPointInt32 Z;

};

} // namespace vehicle::current_location::gnss_receiver::mounting_position

#endif // VEHICLE_CURRENT_LOCATION_GNSS_RECEIVER_MOUNTING_POSITION_MOUNTINGPOSITION_H
