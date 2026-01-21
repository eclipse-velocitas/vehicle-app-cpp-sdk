#ifndef VEHICLE_BODY_WINDSHIELD_REAR_WIPING_SYSTEM_SYSTEM_H
#define VEHICLE_BODY_WINDSHIELD_REAR_WIPING_SYSTEM_SYSTEM_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::body::windshield::rear::wiping::system {
using ParentClass = velocitas::Model;

/** System model. */
class System : public ParentClass {
public:

    System(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		ActualPosition("ActualPosition", this),
		DriveCurrent("DriveCurrent", this),
		Frequency("Frequency", this),
		IsBlocked("IsBlocked", this),
		IsEndingWipeCycle("IsEndingWipeCycle", this),
		IsOverheated("IsOverheated", this),
		IsPositionReached("IsPositionReached", this),
		IsWiperError("IsWiperError", this),
		IsWiping("IsWiping", this),
		Mode("Mode", this),
		TargetPosition("TargetPosition", this)
    {}

    /**
    * ActualPosition: actuator
    * Actual position of main wiper blade for the wiper system relative to reference position. Location of reference position (0 degrees) and direction of positive/negative degrees is vehicle specific.
    *
    * Default parking position might be used as reference position.
    *
    * Unit: degrees
    **/
    velocitas::DataPointFloat ActualPosition;

    /**
    * DriveCurrent: sensor
    * Actual current used by wiper drive.
    *
    * May be negative in special situations.
    *
    * Unit: A
    **/
    velocitas::DataPointFloat DriveCurrent;

    /**
    * Frequency: actuator
    * Wiping frequency/speed, measured in cycles per minute. The signal concerns the actual speed of the wiper blades when moving. Intervals/pauses are excluded, i.e. the value corresponds to the number of cycles that would be completed in 1 minute if wiping permanently over default range.
    *
    * Examples - 0 = Wipers stopped, 80 = Wipers doing 80 cycles per minute (in WIPE mode).
    *
    **/
    velocitas::DataPointUint32 Frequency;

    /**
    * IsBlocked: sensor
    * Indicates if wiper movement is blocked. True = Movement blocked. False = Movement not blocked.
    *
    **/
    velocitas::DataPointBoolean IsBlocked;

    /**
    * IsEndingWipeCycle: sensor
    * Indicates if current wipe movement is completed or near completion. True = Movement is completed or near completion. Changes to RequestedPosition will be executed first after reaching previous RequestedPosition, if it has not already been reached. False = Movement is not near completion. Any change to RequestedPosition will be executed immediately. Change of direction may not be allowed.
    *
    * In continuous wiping between A and B this sensor can be used a trigger to update TargetPosition.
    *
    **/
    velocitas::DataPointBoolean IsEndingWipeCycle;

    /**
    * IsOverheated: sensor
    * Indicates if wiper system is overheated. True = Wiper system overheated. False = Wiper system not overheated.
    *
    **/
    velocitas::DataPointBoolean IsOverheated;

    /**
    * IsPositionReached: sensor
    * Indicates if a requested position has been reached. IsPositionReached refers to the previous position in case the TargetPosition is updated while IsEndingWipeCycle=True. True = Current or Previous TargetPosition reached. False = Position not (yet) reached, or wipers have moved away from the reached position.
    *
    **/
    velocitas::DataPointBoolean IsPositionReached;

    /**
    * IsWiperError: sensor
    * Indicates system failure. True if wiping is disabled due to system failure.
    *
    **/
    velocitas::DataPointBoolean IsWiperError;

    /**
    * IsWiping: sensor
    * Indicates wiper movement. True if wiper blades are moving. Change of direction shall be considered as IsWiping if wipers will continue to move directly after the change of direction.
    *
    **/
    velocitas::DataPointBoolean IsWiping;

    /**
    * Mode: actuator
    * Requested mode of wiper system. STOP_HOLD means that the wipers shall move to position given by TargetPosition and then hold the position. WIPE means that wipers shall move to the position given by TargetPosition and then hold the position if no new TargetPosition is requested. PLANT_MODE means that wiping is disabled. Exact behavior is vehicle specific. EMERGENCY_STOP means that wiping shall be immediately stopped without holding the position.
    *
    * Allowed values: STOP_HOLD, WIPE, PLANT_MODE, EMERGENCY_STOP
    **/
    velocitas::DataPointString Mode;

    /**
    * TargetPosition: actuator
    * Requested position of main wiper blade for the wiper system relative to reference position. Location of reference position (0 degrees) and direction of positive/negative degrees is vehicle specific. System behavior when receiving TargetPosition depends on Mode and IsEndingWipeCycle. Supported values are vehicle specific and might be dynamically corrected. If IsEndingWipeCycle=True then wipers will complete current movement before actuating new TargetPosition. If IsEndingWipeCycle=False then wipers will directly change destination if the TargetPosition is changed.
    *
    * Default parking position might be used as reference position.
    *
    * Unit: degrees
    **/
    velocitas::DataPointFloat TargetPosition;

};

} // namespace vehicle::body::windshield::rear::wiping::system

#endif // VEHICLE_BODY_WINDSHIELD_REAR_WIPING_SYSTEM_SYSTEM_H
