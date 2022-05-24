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

#include "sdk/VehicleApp.h"

#include "sdk/IPubSubClient.h"
#include "sdk/IVehicleDataBrokerClient.h"
#include "sdk/Logger.h"

#include <fmt/core.h>
#include <nlohmann/json.hpp>

#include <chrono>
#include <string>
#include <thread>
#include <utility>

namespace velocitas {

VehicleApp::VehicleApp(std::shared_ptr<IVehicleDataBrokerClient> vdbClient,
                       std::shared_ptr<IPubSubClient>            pubSubClient)
    : m_vdbClient(std::move(vdbClient))
    , m_pubSubClient(std::move(pubSubClient)) {}

void VehicleApp::run() {
    m_pubSubClient->connect();

    logger().info("Running App...");

    onStart();

    // TODO: Fix busy waiting
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void VehicleApp::stop() {
    onStop();

    m_pubSubClient->disconnect();
    logger().info("Stopping App...");
}

AsyncSubscriptionPtr_t<std::string> VehicleApp::subscribeToTopic(const std::string& topic) {
    return m_pubSubClient->subscribeTopic(topic);
}

AsyncResultPtr_t<DataPointsResult>
VehicleApp::getDataPoints(const std::vector<std::reference_wrapper<DataPoint>>& dataPoints) {
    std::vector<std::string> dataPointPaths;
    dataPointPaths.reserve(dataPoints.size());
    std::for_each(dataPoints.begin(), dataPoints.end(),
                  [&dataPointPaths](const std::reference_wrapper<DataPoint>& dataPointRef) {
                      dataPointPaths.emplace_back(dataPointRef.get().getPath());
                  });
    return m_vdbClient->getDatapoints(dataPointPaths);
}

AsyncSubscriptionPtr_t<DataPointsResult> VehicleApp::subscribeDataPoints(const std::string& query) {
    return m_vdbClient->subscribe(query);
}

void VehicleApp::publishToTopic(const std::string& topic, const std::string& data) {
    m_pubSubClient->publishOnTopic(topic, data);
}

std::shared_ptr<IVehicleDataBrokerClient> VehicleApp::getVehicleDataBrokerClient() {
    return m_vdbClient;
}

std::shared_ptr<IPubSubClient> VehicleApp::getPubSubClient() { return m_pubSubClient; }

} // namespace velocitas
