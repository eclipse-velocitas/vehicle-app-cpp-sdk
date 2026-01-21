#ifndef VEHICLE_ADAS_ADAS_H
#define VEHICLE_ADAS_ADAS_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/adas/abs/ABS.hpp"
#include "vehicle/adas/cruise_control/CruiseControl.hpp"
#include "vehicle/adas/eba/EBA.hpp"
#include "vehicle/adas/ebd/EBD.hpp"
#include "vehicle/adas/esc/ESC.hpp"
#include "vehicle/adas/lane_departure_detection/LaneDepartureDetection.hpp"
#include "vehicle/adas/obstacle_detection/ObstacleDetection.hpp"
#include "vehicle/adas/tcs/TCS.hpp"

namespace vehicle::adas {
using ParentClass = velocitas::Model;

/** ADAS model. */
class ADAS : public ParentClass {
public:

    ADAS(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		ABS("ABS", this),
		ActiveAutonomyLevel("ActiveAutonomyLevel", this),
		CruiseControl("CruiseControl", this),
		EBA("EBA", this),
		EBD("EBD", this),
		ESC("ESC", this),
		LaneDepartureDetection("LaneDepartureDetection", this),
		ObstacleDetection("ObstacleDetection", this),
		SupportedAutonomyLevel("SupportedAutonomyLevel", this),
		TCS("TCS", this)
    {}

    /**
    * ABS: branch
    * Antilock Braking System signals.
    *
    **/
    vehicle::adas::abs::ABS ABS;

    /**
    * ActiveAutonomyLevel: sensor
    * Indicates the currently active level of autonomy according to SAE J3016 taxonomy.
    *
    * Follows https://www.sae.org/news/2019/01/sae-updates-j3016-automated-driving-graphic taxonomy. For SAE levels 3 and 4 the system is required to alert the driver before it will disengage. Level 4 systems are required to reach a safe state even if a driver does not take over. Only level 5 systems are required to not rely on a driver at all. While level 2 systems require the driver to be monitoring the system at all times, many level 2 systems, often termed "level 2.5" systems, do warn the driver shortly before reaching their operational limits, therefore we also support the DISENGAGING state for SAE_2.
    *
    * Allowed values: SAE_0, SAE_1, SAE_2_DISENGAGING, SAE_2, SAE_3_DISENGAGING, SAE_3, SAE_4_DISENGAGING, SAE_4, SAE_5
    **/
    velocitas::DataPointString ActiveAutonomyLevel;

    /**
    * CruiseControl: branch
    * Signals from Cruise Control system.
    *
    **/
    vehicle::adas::cruise_control::CruiseControl CruiseControl;

    /**
    * EBA: branch
    * Emergency Brake Assist (EBA) System signals.
    *
    **/
    vehicle::adas::eba::EBA EBA;

    /**
    * EBD: branch
    * Electronic Brakeforce Distribution (EBD) System signals.
    *
    **/
    vehicle::adas::ebd::EBD EBD;

    /**
    * ESC: branch
    * Electronic Stability Control System signals.
    *
    **/
    vehicle::adas::esc::ESC ESC;

    /**
    * LaneDepartureDetection: branch
    * Signals from Lane Departure Detection System.
    *
    **/
    vehicle::adas::lane_departure_detection::LaneDepartureDetection LaneDepartureDetection;

    /**
    * ObstacleDetection: branch
    * Signals form Obstacle Sensor System.
    *
    **/
    vehicle::adas::obstacle_detection::ObstacleDetection ObstacleDetection;

    /**
    * SupportedAutonomyLevel: attribute (string)
    * Indicates the highest level of autonomy according to SAE J3016 taxonomy the vehicle is capable of.
    *
    * Allowed values: SAE_0, SAE_1, SAE_2, SAE_3, SAE_4, SAE_5
    **/
    velocitas::DataPointString SupportedAutonomyLevel;

    /**
    * TCS: branch
    * Traction Control System signals.
    *
    **/
    vehicle::adas::tcs::TCS TCS;

};

} // namespace vehicle::adas

#endif // VEHICLE_ADAS_ADAS_H
