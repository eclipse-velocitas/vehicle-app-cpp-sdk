#ifndef VEHICLE_VEHICLE_IDENTIFICATION_VEHICLEIDENTIFICATION_H
#define VEHICLE_VEHICLE_IDENTIFICATION_VEHICLEIDENTIFICATION_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::vehicle_identification {
using ParentClass = velocitas::Model;

/** VehicleIdentification model. */
class VehicleIdentification : public ParentClass {
public:

    VehicleIdentification(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		AcrissCode("AcrissCode", this),
		BodyType("BodyType", this),
		Brand("Brand", this),
		DateVehicleFirstRegistered("DateVehicleFirstRegistered", this),
		KnownVehicleDamages("KnownVehicleDamages", this),
		MeetsEmissionStandard("MeetsEmissionStandard", this),
		Model("Model", this),
		ProductionDate("ProductionDate", this),
		PurchaseDate("PurchaseDate", this),
		VIN("VIN", this),
		VehicleConfiguration("VehicleConfiguration", this),
		VehicleInteriorColor("VehicleInteriorColor", this),
		VehicleInteriorType("VehicleInteriorType", this),
		VehicleModelDate("VehicleModelDate", this),
		VehicleSeatingCapacity("VehicleSeatingCapacity", this),
		VehicleSpecialUsage("VehicleSpecialUsage", this),
		WMI("WMI", this),
		Year("Year", this)
    {}

    /**
    * AcrissCode: attribute (string)
    * The ACRISS Car Classification Code is a code used by many car rental companies.
    *
    **/
    velocitas::DataPointString AcrissCode;

    /**
    * BodyType: attribute (string)
    * Indicates the design and body style of the vehicle (e.g. station wagon, hatchback, etc.).
    *
    **/
    velocitas::DataPointString BodyType;

    /**
    * Brand: attribute (string)
    * Vehicle brand or manufacturer.
    *
    **/
    velocitas::DataPointString Brand;

    /**
    * DateVehicleFirstRegistered: attribute (string)
    * The date in ISO 8601 format of the first registration of the vehicle with the respective public authorities.
    *
    **/
    velocitas::DataPointString DateVehicleFirstRegistered;

    /**
    * KnownVehicleDamages: attribute (string)
    * A textual description of known damages, both repaired and unrepaired.
    *
    **/
    velocitas::DataPointString KnownVehicleDamages;

    /**
    * MeetsEmissionStandard: attribute (string)
    * Indicates that the vehicle meets the respective emission standard.
    *
    **/
    velocitas::DataPointString MeetsEmissionStandard;

    /**
    * Model: attribute (string)
    * Vehicle model.
    *
    **/
    velocitas::DataPointString Model;

    /**
    * ProductionDate: attribute (string)
    * The date in ISO 8601 format of production of the item, e.g. vehicle.
    *
    **/
    velocitas::DataPointString ProductionDate;

    /**
    * PurchaseDate: attribute (string)
    * The date in ISO 8601 format of the item e.g. vehicle was purchased by the current owner.
    *
    **/
    velocitas::DataPointString PurchaseDate;

    /**
    * VIN: attribute (string)
    * 17-character Vehicle Identification Number (VIN) as defined by ISO 3779.
    *
    **/
    velocitas::DataPointString VIN;

    /**
    * VehicleConfiguration: attribute (string)
    * A short text indicating the configuration of the vehicle, e.g. '5dr hatchback ST 2.5 MT 225 hp' or 'limited edition'.
    *
    **/
    velocitas::DataPointString VehicleConfiguration;

    /**
    * VehicleInteriorColor: attribute (string)
    * The color or color combination of the interior of the vehicle.
    *
    **/
    velocitas::DataPointString VehicleInteriorColor;

    /**
    * VehicleInteriorType: attribute (string)
    * The type or material of the interior of the vehicle (e.g. synthetic fabric, leather, wood, etc.).
    *
    **/
    velocitas::DataPointString VehicleInteriorType;

    /**
    * VehicleModelDate: attribute (string)
    * The release date in ISO 8601 format of a vehicle model (often used to differentiate versions of the same make and model).
    *
    **/
    velocitas::DataPointString VehicleModelDate;

    /**
    * VehicleSeatingCapacity: attribute (uint16)
    * The number of passengers that can be seated in the vehicle, both in terms of the physical space available, and in terms of limitations set by law.
    *
    **/
    velocitas::DataPointUint32 VehicleSeatingCapacity;

    /**
    * VehicleSpecialUsage: attribute (string)
    * Indicates whether the vehicle has been used for special purposes, like commercial rental, driving school.
    *
    **/
    velocitas::DataPointString VehicleSpecialUsage;

    /**
    * WMI: attribute (string)
    * 3-character World Manufacturer Identification (WMI) as defined by ISO 3780.
    *
    **/
    velocitas::DataPointString WMI;

    /**
    * Year: attribute (uint16)
    * Model year of the vehicle.
    *
    **/
    velocitas::DataPointUint32 Year;

};

} // namespace vehicle::vehicle_identification

#endif // VEHICLE_VEHICLE_IDENTIFICATION_VEHICLEIDENTIFICATION_H
