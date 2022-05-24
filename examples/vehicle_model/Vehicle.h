/**
 * Copyright (c) 2022 Robert Bosch GmbH
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

#include "vehicle_model/Cabin/Cabin.h"

namespace velocitas {
/** Vehicle model. */
class Vehicle : public Model {
public:
    Vehicle();

    /**
     * Cabin: branch
     * All in-cabin components, including doors.
     *
     **/
    vehicle::Cabin& getCabin() { return m_cabin; }

    /**
     * Speed: DataPoint
     *
     * Current speed of the vehicle.
     */
    DataPointFloat& getSpeed() { return m_speed; }

    static Vehicle& getInstance();

private:
    DataPointFloat m_speed;

    vehicle::Cabin m_cabin;
};
} // namespace velocitas
#endif // VMDL_EXAMPLE_VEHICLE_H
