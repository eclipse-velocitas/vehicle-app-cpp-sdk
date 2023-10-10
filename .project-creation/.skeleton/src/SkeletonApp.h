/**
 * Copyright (c) 2023 Contributors to the Eclipse Foundation
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

#ifndef VEHICLE_APP_SDK_SKELETON_H
#define VEHICLE_APP_SDK_SKELETON_H

#include "sdk/Status.h"
#include "sdk/VehicleApp.h"
#include "vehicle/Vehicle.hpp"

#include <memory>
#include <string>

namespace skeleton {

/**
 * @brief Skeleton vehicle app.
 */
class SkeletonApp : public velocitas::VehicleApp {
public:
    SkeletonApp();

    /**
     * @brief Run when the vehicle app starts
     *
     */
    void onStart() override;

    /**
     * @brief Handle errors which occurred during async invocation.
     *
     * @param status  The status which contains the error.
     */
    void onError(const velocitas::Status& status);

private:
    vehicle::Vehicle Vehicle;
};

} // namespace skeleton

#endif // VEHICLE_APP_SDK_SKELETON_H
