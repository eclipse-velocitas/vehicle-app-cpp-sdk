#ifndef VEHICLE_CHASSIS_AXLE_ROW_1_ROW1_H
#define VEHICLE_CHASSIS_AXLE_ROW_1_ROW1_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/chassis/axle/row_1/wheel/Wheel.hpp"

#include <string>

namespace vehicle::chassis::axle::row_1 {
using ParentClass = velocitas::Model;

/** Row1 model. */
class Row1 : public ParentClass {
public:

    Row1(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		TireAspectRatio("TireAspectRatio", this),
		TireDiameter("TireDiameter", this),
		TireWidth("TireWidth", this),
		Wheel("Wheel", this),
		WheelCount("WheelCount", this),
		WheelDiameter("WheelDiameter", this),
		WheelWidth("WheelWidth", this)
    {}

    /**
    * TireAspectRatio: attribute (uint8)
    * Aspect ratio between tire section height and tire section width, as per ETRTO / TRA standard.
    *
    * Unit: percent
    **/
    velocitas::DataPointUint32 TireAspectRatio;

    /**
    * TireDiameter: attribute (float)
    * Outer diameter of tires, in inches, as per ETRTO / TRA standard.
    *
    * Unit: inch
    **/
    velocitas::DataPointFloat TireDiameter;

    /**
    * TireWidth: attribute (uint16)
    * Nominal section width of tires, in mm, as per ETRTO / TRA standard.
    *
    * Unit: mm
    **/
    velocitas::DataPointUint32 TireWidth;

    /**
    * Wheel: branch
    * Wheel signals for axle
    *
    **/
    vehicle::chassis::axle::row_1::wheel::Wheel Wheel;

    /**
    * WheelCount: attribute (uint8)
    * Number of wheels on the axle
    *
    **/
    velocitas::DataPointUint32 WheelCount;

    /**
    * WheelDiameter: attribute (float)
    * Diameter of wheels (rims without tires), in inches, as per ETRTO / TRA standard.
    *
    * Unit: inch
    **/
    velocitas::DataPointFloat WheelDiameter;

    /**
    * WheelWidth: attribute (float)
    * Width of wheels (rims without tires), in inches, as per ETRTO / TRA standard.
    *
    * Unit: inch
    **/
    velocitas::DataPointFloat WheelWidth;

};

} // namespace vehicle::chassis::axle::row_1

#endif // VEHICLE_CHASSIS_AXLE_ROW_1_ROW1_H
