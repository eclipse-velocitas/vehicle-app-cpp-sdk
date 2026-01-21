#ifndef VEHICLE_POWERTRAIN_ELECTRIC_MOTOR_ELECTRICMOTOR_H
#define VEHICLE_POWERTRAIN_ELECTRIC_MOTOR_ELECTRICMOTOR_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::powertrain::electric_motor {
using ParentClass = velocitas::Model;

/** ElectricMotor model. */
class ElectricMotor : public ParentClass {
public:

    ElectricMotor(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		CoolantTemperature("CoolantTemperature", this),
		EngineCode("EngineCode", this),
		MaxPower("MaxPower", this),
		MaxRegenPower("MaxRegenPower", this),
		MaxRegenTorque("MaxRegenTorque", this),
		MaxTorque("MaxTorque", this),
		Power("Power", this),
		Speed("Speed", this),
		Temperature("Temperature", this),
		Torque("Torque", this)
    {}

    /**
    * CoolantTemperature: sensor
    * Motor coolant temperature (if applicable).
    *
    * Unit: celsius
    **/
    velocitas::DataPointInt32 CoolantTemperature;

    /**
    * EngineCode: attribute (string)
    * Engine code designation, as specified by vehicle manufacturer.
    *
    **/
    velocitas::DataPointString EngineCode;

    /**
    * MaxPower: attribute (uint16)
    * Peak power, in kilowatts, that motor(s) can generate.
    *
    * Unit: kW
    **/
    velocitas::DataPointUint32 MaxPower;

    /**
    * MaxRegenPower: attribute (uint16)
    * Peak regen/brake power, in kilowatts, that motor(s) can generate.
    *
    * Unit: kW
    **/
    velocitas::DataPointUint32 MaxRegenPower;

    /**
    * MaxRegenTorque: attribute (uint16)
    * Peak regen/brake torque, in newton meter, that the motor(s) can generate.
    *
    * Unit: Nm
    **/
    velocitas::DataPointUint32 MaxRegenTorque;

    /**
    * MaxTorque: attribute (uint16)
    * Peak power, in newton meter, that the motor(s) can generate.
    *
    * Unit: Nm
    **/
    velocitas::DataPointUint32 MaxTorque;

    /**
    * Power: sensor
    * Current motor power output. Negative values indicate regen mode.
    *
    * Unit: kW
    **/
    velocitas::DataPointInt32 Power;

    /**
    * Speed: sensor
    * Motor rotational speed measured as rotations per minute. Negative values indicate reverse driving mode.
    *
    * Unit: rpm
    **/
    velocitas::DataPointInt32 Speed;

    /**
    * Temperature: sensor
    * Motor temperature.
    *
    * Unit: celsius
    **/
    velocitas::DataPointInt32 Temperature;

    /**
    * Torque: sensor
    * Current motor torque. Negative values indicate regen mode.
    *
    * Unit: Nm
    **/
    velocitas::DataPointInt32 Torque;

};

} // namespace vehicle::powertrain::electric_motor

#endif // VEHICLE_POWERTRAIN_ELECTRIC_MOTOR_ELECTRICMOTOR_H
