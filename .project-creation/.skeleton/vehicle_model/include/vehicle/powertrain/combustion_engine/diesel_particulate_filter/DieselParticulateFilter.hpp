#ifndef VEHICLE_POWERTRAIN_COMBUSTION_ENGINE_DIESEL_PARTICULATE_FILTER_DIESELPARTICULATEFILTER_H
#define VEHICLE_POWERTRAIN_COMBUSTION_ENGINE_DIESEL_PARTICULATE_FILTER_DIESELPARTICULATEFILTER_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::powertrain::combustion_engine::diesel_particulate_filter {
using ParentClass = velocitas::Model;

/** DieselParticulateFilter model. */
class DieselParticulateFilter : public ParentClass {
public:

    DieselParticulateFilter(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		DeltaPressure("DeltaPressure", this),
		InletTemperature("InletTemperature", this),
		OutletTemperature("OutletTemperature", this)
    {}

    /**
    * DeltaPressure: sensor
    * Delta Pressure of Diesel Particulate Filter.
    *
    * Unit: Pa
    **/
    velocitas::DataPointFloat DeltaPressure;

    /**
    * InletTemperature: sensor
    * Inlet temperature of Diesel Particulate Filter.
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat InletTemperature;

    /**
    * OutletTemperature: sensor
    * Outlet temperature of Diesel Particulate Filter.
    *
    * Unit: celsius
    **/
    velocitas::DataPointFloat OutletTemperature;

};

} // namespace vehicle::powertrain::combustion_engine::diesel_particulate_filter

#endif // VEHICLE_POWERTRAIN_COMBUSTION_ENGINE_DIESEL_PARTICULATE_FILTER_DIESELPARTICULATEFILTER_H
