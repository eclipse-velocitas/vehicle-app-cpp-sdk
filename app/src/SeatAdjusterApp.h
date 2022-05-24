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

#ifndef VEHICLE_APP_SDK_SEATADJUSTER_EXAMPLE_H
#define VEHICLE_APP_SDK_SEATADJUSTER_EXAMPLE_H

#include "sdk/Logger.h"
#include "sdk/Status.h"
#include "sdk/VehicleApp.h"
#include "vehicle_model/Vehicle.hpp"

#include <memory>
#include <string>

namespace example {

/**
 * @brief A sample SeatAdjusterApp.
 * The SeatAdjusterApp subcribes at the VehicleDataBroker for updates for
 * the Vehicle.Speed signal.It also subscribes at a MQTT topic to listen for
 * incoming requests to change the seat position and calls the SeatService to
 * move the seat upon such a request, but only if Vehicle.Speed equals 0.
 */
class SeatAdjusterApp : public velocitas::VehicleApp {
public:
    SeatAdjusterApp();

    void onStart() override;

    /**
     * @brief Handle speed changed events from the VDB.
     *
     * @param dataPoints  The affected data points.
     */
    void onSpeedChanged(const velocitas::DataPointsResult& dataPoints);

    /**
     * @brief Handle successful seat movement requests.
     *
     * @param requestId           The ID of the request requested the movement.
     * @param requestedPosition   The seat position of the request.
     */
    void onSeatMovementRequested(const velocitas::VoidResult&, int requestId,
                                 float requestedPosition);

    /**
     * @brief Handle set position request from PubSub topic
     *
     * @param data  The JSON string received from PubSub topic.
     */
    void onSetPositionRequestReceived(const std::string& data);

    /**
     * @brief Handle seat movement events from the VDB.
     *
     * @param dataPoints  The affected data points.
     */
    void onSeatPositionChanged(const velocitas::DataPointsResult& dataPoints);

    /**
     * @brief Handle errors which occurred during async invocation.
     *
     * @param status  The status which contains the error.
     */
    void onError(const velocitas::Status& status);

private:
    velocitas::Vehicle Vehicle;
};

} // namespace example

#endif // VEHICLE_APP_SDK_SEATADJUSTER_EXAMPLE_H
