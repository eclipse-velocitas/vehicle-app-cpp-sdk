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

#ifndef VMDL_EXAMPLE_VEHICLE_H
#define VMDL_EXAMPLE_VEHICLE_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/Cabin/Cabin.h"

namespace velocitas {

using ParentClass = Model;

/** Vehicle model. */
class Vehicle : public ParentClass {
public:
    Vehicle()
        : ParentClass("Vehicle")
        , Speed("Speed", this)
        , Cabin("Cabin", this) {}

    /**
     * Speed: sensor
     * Vehicle speed.
     *
     * Unit: km/h
     */
    DataPointFloat Speed;

    /**
     * Cabin: branch
     * All in-cabin components, including doors.
     *
     */
    vehicle::Cabin Cabin;
};

} // namespace velocitas

#endif // VMDL_EXAMPLE_VEHICLE_H
