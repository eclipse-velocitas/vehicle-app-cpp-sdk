/**
 * Copyright (c) 2024-2025 Contributors to the Eclipse Foundation
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

#include <string>
#include <vector>

namespace example {

/**
 * @brief
 */
class PerformanceTestApp : public velocitas::VehicleApp {
public:
    explicit PerformanceTestApp(std::vector<std::string> signalList);

    void onStart() override;

private:
    std::vector<std::string> m_signalList;
};

} // namespace example

#endif // VEHICLE_APP_SDK_PERFORMANCETESTAPP_H
