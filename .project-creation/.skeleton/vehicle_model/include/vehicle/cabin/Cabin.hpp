#ifndef VEHICLE_CABIN_CABIN_H
#define VEHICLE_CABIN_CABIN_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/convertible/Convertible.hpp"
#include "vehicle/cabin/door/Door.hpp"
#include "vehicle/cabin/hvac/HVAC.hpp"
#include "vehicle/cabin/infotainment/Infotainment.hpp"
#include "vehicle/cabin/lights/Lights.hpp"
#include "vehicle/cabin/rear_shade/RearShade.hpp"
#include "vehicle/cabin/rearview_mirror/RearviewMirror.hpp"
#include "vehicle/cabin/seat/Seat.hpp"
#include "vehicle/cabin/sunroof/Sunroof.hpp"

#include <string>

namespace vehicle::cabin {
using ParentClass = velocitas::Model;

/** Cabin model. */
class Cabin : public ParentClass {
public:

    Cabin(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Convertible("Convertible", this),
		Door("Door", this),
		DoorCount("DoorCount", this),
		DriverPosition("DriverPosition", this),
		HVAC("HVAC", this),
		Infotainment("Infotainment", this),
		Lights("Lights", this),
		RearShade("RearShade", this),
		RearviewMirror("RearviewMirror", this),
		Seat("Seat", this),
		SeatPosCount("SeatPosCount", this),
		SeatRowCount("SeatRowCount", this),
		Sunroof("Sunroof", this)
    {}

    /**
    * Convertible: branch
    * Convertible roof.
    *
    **/
    vehicle::cabin::convertible::Convertible Convertible;

    /**
    * Door: branch
    * All doors, including windows and switches.
    *
    **/
    vehicle::cabin::door::Door Door;

    /**
    * DoorCount: attribute (uint8)
    * Number of doors in vehicle.
    *
    **/
    velocitas::DataPointUint32 DoorCount;

    /**
    * DriverPosition: attribute (uint8)
    * The position of the driver seat in row 1.
    *
    * Default value is position 1, i.e. a typical LHD vehicle.
    *
    **/
    velocitas::DataPointUint32 DriverPosition;

    /**
    * HVAC: branch
    * Climate control
    *
    **/
    vehicle::cabin::hvac::HVAC HVAC;

    /**
    * Infotainment: branch
    * Infotainment system.
    *
    **/
    vehicle::cabin::infotainment::Infotainment Infotainment;

    /**
    * Lights: branch
    * Interior lights signals and sensors.
    *
    **/
    vehicle::cabin::lights::Lights Lights;

    /**
    * RearShade: branch
    * Rear window shade.
    *
    **/
    vehicle::cabin::rear_shade::RearShade RearShade;

    /**
    * RearviewMirror: branch
    * Rearview mirror.
    *
    **/
    vehicle::cabin::rearview_mirror::RearviewMirror RearviewMirror;

    /**
    * Seat: branch
    * All seats.
    *
    **/
    vehicle::cabin::seat::Seat Seat;

    /**
    * SeatPosCount: attribute (uint8[])
    * Number of seats across each row from the front to the rear.
    *
    * Default value corresponds to two seats in front row and 3 seats in second row.
    *
    **/
    velocitas::DataPointUint32Array SeatPosCount;

    /**
    * SeatRowCount: attribute (uint8)
    * Number of seat rows in vehicle.
    *
    * Default value corresponds to two rows of seats.
    *
    **/
    velocitas::DataPointUint32 SeatRowCount;

    /**
    * Sunroof: branch
    * Sun roof status.
    *
    **/
    vehicle::cabin::sunroof::Sunroof Sunroof;

};

} // namespace vehicle::cabin

#endif // VEHICLE_CABIN_CABIN_H
