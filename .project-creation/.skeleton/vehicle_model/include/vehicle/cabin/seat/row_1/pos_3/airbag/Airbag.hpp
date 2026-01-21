#ifndef VEHICLE_CABIN_SEAT_ROW_1_POS_3_AIRBAG_AIRBAG_H
#define VEHICLE_CABIN_SEAT_ROW_1_POS_3_AIRBAG_AIRBAG_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::seat::row_1::pos_3::airbag {
using ParentClass = velocitas::Model;

/** Airbag model. */
class Airbag : public ParentClass {
public:

    Airbag(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsDeployed("IsDeployed", this)
    {}

    /**
    * IsDeployed: sensor
    * Airbag deployment status. True = Airbag deployed. False = Airbag not deployed.
    *
    **/
    velocitas::DataPointBoolean IsDeployed;

};

} // namespace vehicle::cabin::seat::row_1::pos_3::airbag

#endif // VEHICLE_CABIN_SEAT_ROW_1_POS_3_AIRBAG_AIRBAG_H
