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

#include "PerformanceTestApp.h"
#include "sdk/IPubSubClient.h"
#include "sdk/Logger.h"
#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <fmt/chrono.h>
#include <fmt/core.h>

#include <chrono>
#include <utility>

namespace example {

namespace {
std::string getValueRepresentation(const velocitas::DataPointValue& value) {
    if (!value.isValid()) {
        return toString(value.getFailure());
    }
    return value.getValueAsString();
}

template <typename TTimeBase> TTimeBase getTimestamp() {
    const auto timestamp      = std::chrono::high_resolution_clock::now();
    const auto timeSinceEpoch = timestamp.time_since_epoch();
    return std::chrono::duration_cast<TTimeBase>(timeSinceEpoch);
}

} // anonymous namespace

PerformanceTestApp::PerformanceTestApp(std::vector<std::string> signalList)
    : VehicleApp(velocitas::IVehicleDataBrokerClient::createInstance("vehicledatabroker"))
    , m_signalList{std::move(signalList)} {}

void PerformanceTestApp::onStart() {
    velocitas::logger().info("Subscribing to signals ...");
    for (auto path : m_signalList) {
        subscribeDataPoints("SELECT " + path)
            ->onItem([path](const velocitas::DataPointReply& reply) {
                const auto value     = getValueRepresentation(*reply.getUntyped(path));
                const auto timestamp = getTimestamp<std::chrono::microseconds>();
                fmt::print("{:%T} - {} - {}\n", timestamp, path, value);
            })
            ->onError([path](const velocitas::Status& status) {
                velocitas::logger().error("Error on subscription for data point {}: {}", path,
                                          status.errorMessage());
            });
    }
}

} // namespace example
