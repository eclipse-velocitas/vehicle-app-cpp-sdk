/**
 * Copyright (c) 2022-2024 Contributors to the Eclipse Foundation
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

#ifndef VEHICLE_APP_SDK_PERFORMANCETESTAPP_H
#define VEHICLE_APP_SDK_PERFORMANCETESTAPP_H

#include "sdk/VehicleApp.h"

#include <memory>

namespace velocitas {
class Vehicle;
} // namespace velocitas

namespace example {

/**
 * @brief
 */
class PerformanceTestApp : public velocitas::VehicleApp {
public:
    PerformanceTestApp();

    void onStart() override;

private:
    std::shared_ptr<velocitas::Vehicle> m_vehicleModel;
};

} // namespace example

#endif // VEHICLE_APP_SDK_PERFORMANCETESTAPP_H
