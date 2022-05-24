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

#include "SeatAdjusterApp.h"
#include "sdk/IPubSubClient.h"
#include "sdk/IVehicleDataBrokerClient.h"
#include "sdk/Logger.h"
#include "sdk/QueryBuilder.h"

#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <utility>

namespace example {

const auto TOPIC_REQUEST          = "seatadjuster/setPosition/request";
const auto TOPIC_RESPONSE         = "seatadjuster/setPosition/response";
const auto TOPIC_CURRENT_POSITION = "seatadjuster/currentPosition";

const auto JSON_FIELD_REQUEST_ID = "requestId";
const auto JSON_FIELD_POSITION   = "position";
const auto JSON_FIELD_STATUS     = "status";
const auto JSON_FIELD_MESSAGE    = "message";
const auto JSON_FIELD_RESULT     = "result";

const auto STATUS_OK   = 0;
const auto STATUS_FAIL = 1;

SeatAdjusterApp::SeatAdjusterApp()
    : VehicleApp(velocitas::IVehicleDataBrokerClient::createInstance("vehicledatabroker"),
                 velocitas::IPubSubClient::createInstance("localhost:1883", "SeatAdjusterApp")) {}

void SeatAdjusterApp::onStart() {
    velocitas::logger().info("Subscribe for Datapoints!");

    const auto& seatPositionDataPoint = Vehicle.Cabin.Seat.Row(1).Pos(1).Position;

    subscribeDataPoints(velocitas::QueryBuilder::select(seatPositionDataPoint).build())
        ->onItem([this](auto&& item) { onSeatPositionChanged(std::forward<decltype(item)>(item)); })
        ->onError([this](auto&& status) { onError(std::forward<decltype(status)>(status)); });

    // TODO: Do we need some kind of functionality to ensure that the Model
    // in the VDB is actually the same as the one we are using? i.e. does it
    // have the data points we are expecting???
    // m_vehicleModel->verify();

    subscribeToTopic(TOPIC_REQUEST)
        ->onItem([this](auto&& item) {
            onSetPositionRequestReceived(std::forward<decltype(item)>(item));
        })
        ->onError([this](auto&& status) { onError(std::forward<decltype(status)>(status)); });
}

void SeatAdjusterApp::onSpeedChanged(const velocitas::DataPointsResult& result) {
    velocitas::logger().info(
        fmt::format("Speed has changed: {}", result.get(Vehicle.Speed)->value()));
}

void SeatAdjusterApp::onSeatMovementRequested(const velocitas::VoidResult& status, int requestId,
                                              float requestedPosition) {
    velocitas::logger().info("Seat movement request processed...");

    nlohmann::json respData(
        {{JSON_FIELD_REQUEST_ID, requestId},
         {JSON_FIELD_RESULT,
          {{JSON_FIELD_MESSAGE, fmt::format("Called MoveComponent {}", requestedPosition)},
           {JSON_FIELD_STATUS, STATUS_OK}}}});
    publishToTopic(TOPIC_RESPONSE, respData.dump());
}

void SeatAdjusterApp::onSetPositionRequestReceived(const std::string& data) {
    const auto jsonData = nlohmann::json::parse(data);
    if (!jsonData.contains(JSON_FIELD_POSITION)) {
        const auto errorMsg = fmt::format("No position specified");
        velocitas::logger().error(errorMsg);

        nlohmann::json respData({{JSON_FIELD_REQUEST_ID, jsonData[JSON_FIELD_REQUEST_ID]},
                                 {JSON_FIELD_STATUS, STATUS_FAIL},
                                 {JSON_FIELD_MESSAGE, errorMsg}});
        publishToTopic(TOPIC_RESPONSE, respData.dump());
        return;
    }

    const auto desiredSeatPosition = jsonData[JSON_FIELD_POSITION].get<int>();
    const auto requestId           = jsonData[JSON_FIELD_REQUEST_ID].get<int>();

    const auto result = getDataPoints({Vehicle.Speed})->await();

    auto       vehicleSpeed          = 0.F;
    const auto vehicleSpeedDataPoint = result.get(Vehicle.Speed);
    if (!vehicleSpeedDataPoint->isValid()) {
        velocitas::logger().error(fmt::format("{} is not a valid data point: {}",
                                              vehicleSpeedDataPoint->getPath(),
                                              vehicleSpeedDataPoint->asFailure().getReason()));
    }

    vehicleSpeed = vehicleSpeedDataPoint->value();
    if (vehicleSpeed == 0) {
        velocitas::vehicle::cabin::SeatService::SeatLocation location{1, 1};

        Vehicle.Cabin.SeatService
            .moveComponent(location, velocitas::vehicle::cabin::SeatService::Component::Base,
                           desiredSeatPosition)
            ->onResult([this, requestId, desiredSeatPosition](auto&& result) {
                onSeatMovementRequested(std::forward<decltype(result)>(result), requestId,
                                        desiredSeatPosition);
            })
            ->onError([this](auto&& status) { onError(std::forward<decltype(status)>(status)); });
    } else {
        const auto errorMsg = fmt::format(
            "Not allowed to move seat because vehicle speed is {} and not 0", vehicleSpeed);
        velocitas::logger().info(errorMsg);

        nlohmann::json respData(
            {{JSON_FIELD_REQUEST_ID, requestId},
             {JSON_FIELD_RESULT,
              {{JSON_FIELD_STATUS, STATUS_FAIL}, {JSON_FIELD_MESSAGE, errorMsg}}}});
        publishToTopic(TOPIC_RESPONSE, respData.dump());
    }
}

void SeatAdjusterApp::onSeatPositionChanged(const velocitas::DataPointsResult& result) {
    const auto seatPosition = result.get(Vehicle.Cabin.Seat.Row(1).Pos(1).Position);

    if (!seatPosition->isValid()) {
        velocitas::logger().error(fmt::format(R"(DataPoint "{}" caused a failure: "{}"!)",
                                              seatPosition->getPath(),
                                              seatPosition->asFailure().getReason()));
    }

    try {
        nlohmann::json jsonResponse({JSON_FIELD_POSITION, seatPosition->value()});

        publishToTopic(TOPIC_CURRENT_POSITION, jsonResponse.dump());
    } catch (std::exception& exception) {
        velocitas::logger().error(
            fmt::format("Unable to get Current Seat Position, Exception: {}", exception.what()));
        nlohmann::json jsonResponse(
            {{JSON_FIELD_STATUS, STATUS_FAIL}, {JSON_FIELD_MESSAGE, exception.what()}});

        publishToTopic(TOPIC_CURRENT_POSITION, jsonResponse.dump());
    }
}

void SeatAdjusterApp::onError(const velocitas::Status& status) {
    velocitas::logger().error(
        fmt::format("Error occurred during async invocation: {}", status.errorMessage()));
}

} // namespace example
