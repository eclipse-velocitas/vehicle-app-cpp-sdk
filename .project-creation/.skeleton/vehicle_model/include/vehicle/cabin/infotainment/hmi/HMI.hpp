#ifndef VEHICLE_CABIN_INFOTAINMENT_HMI_HMI_H
#define VEHICLE_CABIN_INFOTAINMENT_HMI_HMI_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::infotainment::hmi {
using ParentClass = velocitas::Model;

/** HMI model. */
class HMI : public ParentClass {
public:

    HMI(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		CurrentLanguage("CurrentLanguage", this),
		DateFormat("DateFormat", this),
		DayNightMode("DayNightMode", this),
		DistanceUnit("DistanceUnit", this),
		EVEconomyUnits("EVEconomyUnits", this),
		FuelEconomyUnits("FuelEconomyUnits", this),
		TemperatureUnit("TemperatureUnit", this),
		TimeFormat("TimeFormat", this)
    {}

    /**
    * CurrentLanguage: sensor
    * ISO 639-1 standard language code for the current HMI
    *
    **/
    velocitas::DataPointString CurrentLanguage;

    /**
    * DateFormat: actuator
    * Date format used in the current HMI
    *
    * Allowed values: YYYY_MM_DD, DD_MM_YYYY, MM_DD_YYYY, YY_MM_DD, DD_MM_YY, MM_DD_YY
    **/
    velocitas::DataPointString DateFormat;

    /**
    * DayNightMode: actuator
    * Current display theme
    *
    * Allowed values: DAY, NIGHT
    **/
    velocitas::DataPointString DayNightMode;

    /**
    * DistanceUnit: actuator
    * Distance unit used in the current HMI
    *
    * Allowed values: MILES, KILOMETERS
    **/
    velocitas::DataPointString DistanceUnit;

    /**
    * EVEconomyUnits: actuator
    * EV fuel economy unit used in the current HMI
    *
    * Allowed values: MILES_PER_KILOWATT_HOUR, KILOMETERS_PER_KILOWATT_HOUR, KILOWATT_HOURS_PER_100_MILES, KILOWATT_HOURS_PER_100_KILOMETERS, WATT_HOURS_PER_MILE, WATT_HOURS_PER_KILOMETER
    **/
    velocitas::DataPointString EVEconomyUnits;

    /**
    * FuelEconomyUnits: actuator
    * Fuel economy unit used in the current HMI
    *
    * Allowed values: MPG_UK, MPG_US, MILES_PER_LITER, KILOMETERS_PER_LITER, LITERS_PER_100_KILOMETERS
    **/
    velocitas::DataPointString FuelEconomyUnits;

    /**
    * TemperatureUnit: actuator
    * Temperature unit used in the current HMI
    *
    * Allowed values: C, F
    **/
    velocitas::DataPointString TemperatureUnit;

    /**
    * TimeFormat: actuator
    * Time format used in the current HMI
    *
    * Allowed values: HR_12, HR_24
    **/
    velocitas::DataPointString TimeFormat;

};

} // namespace vehicle::cabin::infotainment::hmi

#endif // VEHICLE_CABIN_INFOTAINMENT_HMI_HMI_H
