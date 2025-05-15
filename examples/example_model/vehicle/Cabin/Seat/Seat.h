/**
 * Copyright (c) 2022-2025 Contributors to the Eclipse Foundation
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef VMDL_EXAMPLE_VEHICLE_CABIN_SEAT_H
#define VMDL_EXAMPLE_VEHICLE_CABIN_SEAT_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

namespace velocitas::vehicle::cabin {

using ParentClass = Model;

/** Seat model. */
class Seat : public ParentClass {
public:
    Seat(const std::string& name, ParentClass* parent)
        : ParentClass(name, parent)
        , Position("Position", this) {}

    /**
     * Position: actuator
     * Seat position on vehicle x-axis. Position is relative to the frontmost position supported by
     *the seat. 0 = Frontmost position supported.
     *
     * Value range: [0, ]
     * Unit: mm
     **/
    DataPointUint32 Position;
};

} // namespace velocitas::vehicle::cabin

#endif // VMDL_EXAMPLE_VEHICLE_CABIN_SEAT_H
