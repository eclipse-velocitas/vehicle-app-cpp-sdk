#ifndef VEHICLE_POWERTRAIN_TRACTION_BATTERY_TEMPERATURE_TEMPERATURE_H
#define VEHICLE_POWERTRAIN_TRACTION_BATTERY_TEMPERATURE_TEMPERATURE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::powertrain::traction_battery::temperature {
using ParentClass = velocitas::Model;

/** Temperature model. */
class Temperature : public ParentClass {
public:

    Temperature(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Average("Average", this),
		Max("Max", this),
		Min("Min", this)
    {}

    /**
    * Average: sensor
    * Current average temperature of the battery cells.
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat Average;

    /**
    * Max: sensor
    * Current maximum temperature of the battery cells, i.e. temperature of the hottest cell.
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat Max;

    /**
    * Min: sensor
    * Current minimum temperature of the battery cells, i.e. temperature of the coldest cell.
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat Min;

};

} // namespace vehicle::powertrain::traction_battery::temperature

#endif // VEHICLE_POWERTRAIN_TRACTION_BATTERY_TEMPERATURE_TEMPERATURE_H
