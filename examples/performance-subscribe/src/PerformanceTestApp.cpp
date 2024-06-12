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

#include "PerformanceTestApp.h"
#include "sdk/IPubSubClient.h"
#include "sdk/Logger.h"
#include "sdk/QueryBuilder.h"
#include "sdk/vdb/IVehicleDataBrokerClient.h"
#include "vehicle_model/Vehicle.h"

#include <csignal>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

namespace example {

const auto JSON_FIELD_REQUEST_ID = "requestId";
const auto JSON_FIELD_POSITION   = "position";
const auto JSON_FIELD_STATUS     = "status";
const auto JSON_FIELD_MESSAGE    = "message";
const auto JSON_FIELD_RESULT     = "result";

namespace {
std::string getValueRepresentation(const velocitas::DataPointValue& value) {
    if (!value.isValid()) {
        switch (value.getFailure()) {
        case velocitas::DataPointValue::Failure::INVALID_VALUE:
            return "Failure::INVALID_VALUE";
        case velocitas::DataPointValue::Failure::NOT_AVAILABLE:
            return "Failure::NOT_AVAILABLE";
        case velocitas::DataPointValue::Failure::UNKNOWN_DATAPOINT:
            return "Failure::UNKNOWN_DATAPOINT";
        case velocitas::DataPointValue::Failure::ACCESS_DENIED:
            return "Failure::ACCESS_DENIED";
        case velocitas::DataPointValue::Failure::INTERNAL_ERROR:
        default:
            return "Failure::INTERNAL_ERROR";
        }
    }

    std::ostringstream oss;
    switch (value.getType()) {
    case velocitas::DataPointValue::Type::BOOL:
        oss << dynamic_cast<const velocitas::TypedDataPointValue<bool>&>(value).value();
        break;
    case velocitas::DataPointValue::Type::BOOL_ARRAY:
        break;
    case velocitas::DataPointValue::Type::INT32:
        oss << dynamic_cast<const velocitas::TypedDataPointValue<int32_t>&>(value).value();
        break;
    case velocitas::DataPointValue::Type::INT32_ARRAY:
        break;
    case velocitas::DataPointValue::Type::INT64:
        oss << dynamic_cast<const velocitas::TypedDataPointValue<int64_t>&>(value).value();
        break;
    case velocitas::DataPointValue::Type::INT64_ARRAY:
        break;
    case velocitas::DataPointValue::Type::UINT32:
        oss << dynamic_cast<const velocitas::TypedDataPointValue<uint32_t>&>(value).value();
        break;
    case velocitas::DataPointValue::Type::UINT32_ARRAY:
        break;
    case velocitas::DataPointValue::Type::UINT64:
        oss << dynamic_cast<const velocitas::TypedDataPointValue<uint64_t>&>(value).value();
        break;
    case velocitas::DataPointValue::Type::UINT64_ARRAY:
        break;
    case velocitas::DataPointValue::Type::FLOAT:
        oss << dynamic_cast<const velocitas::TypedDataPointValue<float>&>(value).value();
        break;
    case velocitas::DataPointValue::Type::FLOAT_ARRAY:
        break;
    case velocitas::DataPointValue::Type::DOUBLE:
        oss << dynamic_cast<const velocitas::TypedDataPointValue<double>&>(value).value();
        break;
    case velocitas::DataPointValue::Type::DOUBLE_ARRAY:
        break;
    case velocitas::DataPointValue::Type::STRING:
        oss << dynamic_cast<const velocitas::TypedDataPointValue<std::string>&>(value).value();
        break;
    case velocitas::DataPointValue::Type::STRING_ARRAY:
        break;
    default:
        oss << "UNKNOWN VALUE TYPE";
        break;
    }
    return oss.str();
}

} // anonymous namespace

PerformanceTestApp::PerformanceTestApp()
    : VehicleApp(velocitas::IVehicleDataBrokerClient::createInstance("vehicledatabroker"),
                 velocitas::IPubSubClient::createInstance("PerformanceTestApp"))
    , m_vehicleModel(std::make_shared<velocitas::Vehicle>()) {}

void PerformanceTestApp::onStart() {
    velocitas::logger().info("Subscribe for data points!");

    std::vector<std::string> dataPointList{"Speed", "Something", "IsMoving"};

    for (auto dataPointName : dataPointList) {
        const velocitas::DataPoint* dataPoint = m_vehicleModel->getDataPoint(dataPointName);
        if (dataPoint != nullptr) {
            auto path = dataPoint->getPath();
            subscribeDataPoints(velocitas::QueryBuilder::select(*dataPoint).build())
                ->onItem([path](const velocitas::DataPointReply& reply) {
                    auto        value     = getValueRepresentation(*reply.getGeneric(path));
                    auto        timestamp = std::chrono::high_resolution_clock::now();
                    std::time_t time      = std::chrono::system_clock::to_time_t(timestamp);
                    velocitas::logger().info("{} - {} - {}", std::ctime(&time), path, value);
                })
                ->onError([path](velocitas::Status status) {
                    velocitas::logger().error("Error on subscription for data point {}: {}", path,
                                              status.errorMessage());
                });
        } else {
            velocitas::logger().error("Undefined data point: {}", dataPointName);
        }
    }
}

} // namespace example

std::unique_ptr<example::PerformanceTestApp> myApp;

void signal_handler(int sig) {
    velocitas::logger().info("App terminating signal received: {}", sig);
    myApp->stop();
}

int main(int argc, char** argv) {
    signal(SIGINT, signal_handler);

    myApp = std::make_unique<example::PerformanceTestApp>();
    myApp->run();
    return 0;
}
