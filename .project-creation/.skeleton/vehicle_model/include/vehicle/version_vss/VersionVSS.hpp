#ifndef VEHICLE_VERSION_VSS_VERSIONVSS_H
#define VEHICLE_VERSION_VSS_VERSIONVSS_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::version_vss {
using ParentClass = velocitas::Model;

/** VersionVSS model. */
class VersionVSS : public ParentClass {
public:

    VersionVSS(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Label("Label", this),
		Major("Major", this),
		Minor("Minor", this),
		Patch("Patch", this)
    {}

    /**
    * Label: attribute (string)
    * Label to further describe the version.
    *
    **/
    velocitas::DataPointString Label;

    /**
    * Major: attribute (uint32)
    * Supported Version of VSS - Major version.
    *
    **/
    velocitas::DataPointUint32 Major;

    /**
    * Minor: attribute (uint32)
    * Supported Version of VSS - Minor version.
    *
    **/
    velocitas::DataPointUint32 Minor;

    /**
    * Patch: attribute (uint32)
    * Supported Version of VSS - Patch version.
    *
    **/
    velocitas::DataPointUint32 Patch;

};

} // namespace vehicle::version_vss

#endif // VEHICLE_VERSION_VSS_VERSIONVSS_H
