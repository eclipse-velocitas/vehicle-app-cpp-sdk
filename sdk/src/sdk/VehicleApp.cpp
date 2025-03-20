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

#include "sdk/VehicleApp.h"

#include "sdk/IPubSubClient.h"
#include "sdk/Logger.h"
#include "sdk/VehicleModelContext.h"
#include "sdk/middleware/Middleware.h"
#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <fmt/core.h>
#include <nlohmann/json.hpp>

#include <string>

namespace velocitas {

VehicleApp::VehicleApp(std::shared_ptr<IVehicleDataBrokerClient> vdbClient,
                       std::shared_ptr<IPubSubClient>            pubSubClient)
    : m_vdbClient(vdbClient)
    , m_pubSubClient(std::move(pubSubClient)) {
    VehicleModelContext::getInstance().setVdbc(vdbClient);
}

void VehicleApp::run() {
    logger().info("Starting app ...");
    Middleware::getInstance().start();
    Middleware::getInstance().waitUntilReady();

    if (m_pubSubClient) {
        m_pubSubClient->connect();
    }
    onStart();
    {
        std::unique_lock lk(m_stopWaitMutex);
        m_isRunning = true;
    }
    logger().info("App is running.");

    {
        std::unique_lock lk(m_stopWaitMutex);
        m_stopWaitCV.wait(lk, [this] { return !m_isRunning; });
    }
    logger().info("App stopped.");
}

void VehicleApp::stop() {
    logger().info("Stopping app ...");

    onStop();
    if (m_pubSubClient) {
        m_pubSubClient->disconnect();
    }
    Middleware::getInstance().stop();

    {
        std::unique_lock lock(m_stopWaitMutex);
        m_isRunning = false;
        m_stopWaitCV.notify_all();
    }
}

AsyncSubscriptionPtr_t<std::string> VehicleApp::subscribeToTopic(const std::string& topic) {
    if (m_pubSubClient) {
        return m_pubSubClient->subscribeTopic(topic);
    }
    logger().error("subscribeToTopic(...) disfunctional: App has no PubSubClient instantiated");
    return {};
}

AsyncResultPtr_t<DataPointReply>
VehicleApp::getDataPoints(const std::vector<std::reference_wrapper<DataPoint>>& dataPoints) {
    std::vector<std::string> dataPointPaths;
    dataPointPaths.reserve(dataPoints.size());
    std::for_each(dataPoints.begin(), dataPoints.end(),
                  [&dataPointPaths](const std::reference_wrapper<DataPoint>& dataPointRef) {
                      dataPointPaths.emplace_back(dataPointRef.get().getPath());
                  });
    return m_vdbClient->getDatapoints(dataPointPaths);
}

AsyncResultPtr_t<DataPointReply>
VehicleApp::getDataPoint_internal(const DataPoint& dataPoint) const {
    std::vector<std::string> dataPointPaths;
    dataPointPaths.reserve(1);
    dataPointPaths.emplace_back(dataPoint.getPath());
    return m_vdbClient->getDatapoints(dataPointPaths);
}

AsyncSubscriptionPtr_t<DataPointReply> VehicleApp::subscribeDataPoints(const std::string& query) {
    return m_vdbClient->subscribe(query);
}

void VehicleApp::publishToTopic(const std::string& topic, const std::string& data) {
    if (m_pubSubClient) {
        m_pubSubClient->publishOnTopic(topic, data);
    } else {
        logger().error("publishToTopic(...) ignored: App has no PubSubClient instantiated");
    }
}

std::shared_ptr<IVehicleDataBrokerClient> VehicleApp::getVehicleDataBrokerClient() {
    return m_vdbClient;
}

std::shared_ptr<IPubSubClient> VehicleApp::getPubSubClient() { return m_pubSubClient; }

} // namespace velocitas
