#ifndef VEHICLE_VEHICLE_H
#define VEHICLE_VEHICLE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/acceleration/Acceleration.hpp"
#include "vehicle/adas/ADAS.hpp"
#include "vehicle/angular_velocity/AngularVelocity.hpp"
#include "vehicle/body/Body.hpp"
#include "vehicle/cabin/Cabin.hpp"
#include "vehicle/chassis/Chassis.hpp"
#include "vehicle/connectivity/Connectivity.hpp"
#include "vehicle/current_location/CurrentLocation.hpp"
#include "vehicle/driver/Driver.hpp"
#include "vehicle/exterior/Exterior.hpp"
#include "vehicle/obd/OBD.hpp"
#include "vehicle/powertrain/Powertrain.hpp"
#include "vehicle/service/Service.hpp"
#include "vehicle/trailer/Trailer.hpp"
#include "vehicle/vehicle_identification/VehicleIdentification.hpp"
#include "vehicle/version_vss/VersionVSS.hpp"

namespace vehicle {
using ParentClass = velocitas::Model;

/** Vehicle model. */
class Vehicle : public ParentClass {
public:

    Vehicle() :
        ParentClass("Vehicle"),
		ADAS("ADAS", this),
		Acceleration("Acceleration", this),
		AngularVelocity("AngularVelocity", this),
		AverageSpeed("AverageSpeed", this),
		Body("Body", this),
		Cabin("Cabin", this),
		CargoVolume("CargoVolume", this),
		Chassis("Chassis", this),
		Connectivity("Connectivity", this),
		CurbWeight("CurbWeight", this),
		CurrentLocation("CurrentLocation", this),
		CurrentOverallWeight("CurrentOverallWeight", this),
		Driver("Driver", this),
		EmissionsCO2("EmissionsCO2", this),
		Exterior("Exterior", this),
		GrossWeight("GrossWeight", this),
		Height("Height", this),
		IsBrokenDown("IsBrokenDown", this),
		IsMoving("IsMoving", this),
		Length("Length", this),
		LowVoltageSystemState("LowVoltageSystemState", this),
		MaxTowBallWeight("MaxTowBallWeight", this),
		MaxTowWeight("MaxTowWeight", this),
		OBD("OBD", this),
		Powertrain("Powertrain", this),
		RoofLoad("RoofLoad", this),
		Service("Service", this),
		Speed("Speed", this),
		Trailer("Trailer", this),
		TravelledDistance("TravelledDistance", this),
		TripMeterReading("TripMeterReading", this),
		VehicleIdentification("VehicleIdentification", this),
		VersionVSS("VersionVSS", this),
		Width("Width", this)
    {}

    /**
    * ADAS: branch
    * All Advanced Driver Assist Systems data.
    *
    **/
    vehicle::adas::ADAS ADAS;

    /**
    * Acceleration: branch
    * Spatial acceleration. Axis definitions according to ISO 8855.
    *
    **/
    vehicle::acceleration::Acceleration Acceleration;

    /**
    * AngularVelocity: branch
    * Spatial rotation. Axis definitions according to ISO 8855.
    *
    **/
    vehicle::angular_velocity::AngularVelocity AngularVelocity;

    /**
    * AverageSpeed: sensor
    * Average speed for the current trip.
    *
    * Unit: km/h
    **/
    velocitas::DataPointFloat AverageSpeed;

    /**
    * Body: branch
    * All body components.
    *
    **/
    vehicle::body::Body Body;

    /**
    * Cabin: branch
    * All in-cabin components, including doors.
    *
    **/
    vehicle::cabin::Cabin Cabin;

    /**
    * CargoVolume: attribute (float)
    * The available volume for cargo or luggage. For automobiles, this is usually the trunk volume.
    *
    * Value range: [0, ]
    * Unit: l
    **/
    velocitas::DataPointFloat CargoVolume;

    /**
    * Chassis: branch
    * All data concerning steering, suspension, wheels, and brakes.
    *
    **/
    vehicle::chassis::Chassis Chassis;

    /**
    * Connectivity: branch
    * Connectivity data.
    *
    **/
    vehicle::connectivity::Connectivity Connectivity;

    /**
    * CurbWeight: attribute (uint16)
    * Vehicle curb weight, including all liquids and full tank of fuel, but no cargo or passengers.
    *
    * Unit: kg
    **/
    velocitas::DataPointUint32 CurbWeight;

    /**
    * CurrentLocation: branch
    * The current latitude and longitude of the vehicle.
    *
    **/
    vehicle::current_location::CurrentLocation CurrentLocation;

    /**
    * CurrentOverallWeight: sensor
    * Current overall Vehicle weight. Including passengers, cargo and other load inside the car.
    *
    * Unit: kg
    **/
    velocitas::DataPointUint32 CurrentOverallWeight;

    /**
    * Driver: branch
    * Driver data.
    *
    **/
    vehicle::driver::Driver Driver;

    /**
    * EmissionsCO2: attribute (int16)
    * The CO2 emissions.
    *
    * Unit: g/km
    **/
    velocitas::DataPointInt32 EmissionsCO2;

    /**
    * Exterior: branch
    * Information about exterior measured by vehicle.
    *
    **/
    vehicle::exterior::Exterior Exterior;

    /**
    * GrossWeight: attribute (uint16)
    * Curb weight of vehicle, including all liquids and full tank of fuel and full load of cargo and passengers.
    *
    * Unit: kg
    **/
    velocitas::DataPointUint32 GrossWeight;

    /**
    * Height: attribute (uint16)
    * Overall vehicle height.
    *
    * Unit: mm
    **/
    velocitas::DataPointUint32 Height;

    /**
    * IsBrokenDown: sensor
    * Vehicle breakdown or any similar event causing vehicle to stop on the road, that might pose a risk to other road users. True = Vehicle broken down on the road, due to e.g. engine problems, flat tire, out of gas, brake problems. False = Vehicle not broken down.
    *
    * Actual criteria and method used to decide if a vehicle is broken down is implementation specific.
    *
    **/
    velocitas::DataPointBoolean IsBrokenDown;

    /**
    * IsMoving: sensor
    * Indicates whether the vehicle is stationary or moving.
    *
    **/
    velocitas::DataPointBoolean IsMoving;

    /**
    * Length: attribute (uint16)
    * Overall vehicle length.
    *
    * Unit: mm
    **/
    velocitas::DataPointUint32 Length;

    /**
    * LowVoltageSystemState: sensor
    * State of the supply voltage of the control units (usually 12V).
    *
    * Allowed values: UNDEFINED, LOCK, OFF, ACC, ON, START
    **/
    velocitas::DataPointString LowVoltageSystemState;

    /**
    * MaxTowBallWeight: attribute (uint16)
    * Maximum vertical weight on the tow ball of a trailer.
    *
    * Unit: kg
    **/
    velocitas::DataPointUint32 MaxTowBallWeight;

    /**
    * MaxTowWeight: attribute (uint16)
    * Maximum weight of trailer.
    *
    * Unit: kg
    **/
    velocitas::DataPointUint32 MaxTowWeight;

    /**
    * OBD: branch
    * OBD data.
    *
    **/
    vehicle::obd::OBD OBD;

    /**
    * Powertrain: branch
    * Powertrain data for battery management, etc.
    *
    **/
    vehicle::powertrain::Powertrain Powertrain;

    /**
    * RoofLoad: attribute (int16)
    * The permitted total weight of cargo and installations (e.g. a roof rack) on top of the vehicle.
    *
    * Unit: kg
    **/
    velocitas::DataPointInt32 RoofLoad;

    /**
    * Service: branch
    * Service data.
    *
    **/
    vehicle::service::Service Service;

    /**
    * Speed: sensor
    * Vehicle speed.
    *
    * Unit: km/h
    **/
    velocitas::DataPointFloat Speed;

    /**
    * Trailer: branch
    * Trailer signals.
    *
    **/
    vehicle::trailer::Trailer Trailer;

    /**
    * TravelledDistance: sensor
    * Odometer reading, total distance travelled during the lifetime of the vehicle.
    *
    * Unit: km
    **/
    velocitas::DataPointFloat TravelledDistance;

    /**
    * TripMeterReading: sensor
    * Current trip meter reading.
    *
    * Unit: km
    **/
    velocitas::DataPointFloat TripMeterReading;

    /**
    * VehicleIdentification: branch
    * Attributes that identify a vehicle.
    *
    **/
    vehicle::vehicle_identification::VehicleIdentification VehicleIdentification;

    /**
    * VersionVSS: branch
    * Supported Version of VSS.
    *
    **/
    vehicle::version_vss::VersionVSS VersionVSS;

    /**
    * Width: attribute (uint16)
    * Overall vehicle width.
    *
    * Unit: mm
    **/
    velocitas::DataPointUint32 Width;

};

} // namespace vehicle

#endif // VEHICLE_VEHICLE_H
