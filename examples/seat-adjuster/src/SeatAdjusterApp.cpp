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

#include "SeatAdjusterApp.h"
#include "sdk/IPubSubClient.h"
#include "sdk/Logger.h"
#include "sdk/QueryBuilder.h"
#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <csignal>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

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
                 velocitas::IPubSubClient::createInstance("SeatAdjusterApp"))
    , m_vehicleModel(std::make_shared<velocitas::Vehicle>()) {}

void SeatAdjusterApp::onStart() {
    velocitas::logger().info("Subscribe for data points!");

    subscribeDataPoints(
        velocitas::QueryBuilder::select(m_vehicleModel->Cabin.Seat.Row1.DriverSide.Position)
            .build())
        ->onItem([this](auto&& item) { onSeatPositionChanged(std::forward<decltype(item)>(item)); })
        ->onError(
            [this](auto&& status) { onErrorDatapoint(std::forward<decltype(status)>(status)); });

    subscribeToTopic(TOPIC_REQUEST)
        ->onItem([this](auto&& item) {
            onSetPositionRequestReceived(std::forward<decltype(item)>(item));
        })
        ->onError([this](auto&& status) { onErrorTopic(std::forward<decltype(status)>(status)); });
}

void SeatAdjusterApp::onSetPositionRequestReceived(const std::string& data) {
    velocitas::logger().debug("position request: \"{}\"", data);
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

    nlohmann::json respData({{JSON_FIELD_REQUEST_ID, requestId}, {JSON_FIELD_RESULT, {}}});
    const auto     vehicleSpeed = m_vehicleModel->Speed.get()->await().value();
    if (vehicleSpeed == 0) {
        m_vehicleModel->Cabin.Seat.Row1.DriverSide.Position.set(desiredSeatPosition)->await();

        respData[JSON_FIELD_RESULT][JSON_FIELD_STATUS] = STATUS_OK;
        respData[JSON_FIELD_RESULT][JSON_FIELD_MESSAGE] =
            fmt::format("Set Seat position to: {}", desiredSeatPosition);
    } else {
        const auto errorMsg = fmt::format(
            "Not allowed to move seat because vehicle speed is {} and not 0", vehicleSpeed);
        velocitas::logger().info(errorMsg);

        respData[JSON_FIELD_RESULT][JSON_FIELD_STATUS]  = STATUS_FAIL;
        respData[JSON_FIELD_RESULT][JSON_FIELD_MESSAGE] = errorMsg;
    }

    publishToTopic(TOPIC_RESPONSE, respData.dump());
}

void SeatAdjusterApp::onSeatPositionChanged(const velocitas::DataPointReply& dataPoints) {
    nlohmann::json jsonResponse;
    try {
        const auto seatPositionValue =
            dataPoints.get(m_vehicleModel->Cabin.Seat.Row1.DriverSide.Position)->value();
        jsonResponse[JSON_FIELD_POSITION] = seatPositionValue;
    } catch (std::exception& exception) {
        velocitas::logger().warn("Unable to get Current Seat Position, Exception: {}",
                                 exception.what());
        jsonResponse[JSON_FIELD_STATUS]  = STATUS_FAIL;
        jsonResponse[JSON_FIELD_MESSAGE] = exception.what();
    }

    publishToTopic(TOPIC_CURRENT_POSITION, jsonResponse.dump());
}

void SeatAdjusterApp::onError(const velocitas::Status& status) {
    velocitas::logger().error("Error occurred during async invocation: {}", status.errorMessage());
}

void SeatAdjusterApp::onErrorDatapoint(const velocitas::Status& status) {
    velocitas::logger().error("Datapoint: Error occurred during async invocation: {}",
                              status.errorMessage());
}
void SeatAdjusterApp::onErrorTopic(const velocitas::Status& status) {
    velocitas::logger().error("Topic: Error occurred during async invocation: {}",
                              status.errorMessage());
}
} // namespace example

std::unique_ptr<example::SeatAdjusterApp> myApp;

void signal_handler(int sig) {
    velocitas::logger().info("App terminating signal received: {}", sig);
    myApp->stop();
}

int main(int argc, char** argv) {
    signal(SIGINT, signal_handler);

    myApp = std::make_unique<example::SeatAdjusterApp>();
    myApp->run();
    return 0;
}
