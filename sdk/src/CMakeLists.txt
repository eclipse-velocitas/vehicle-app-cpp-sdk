# Copyright (c) 2022-2024 Contributors to the Eclipse Foundation
#
# This program and the accompanying materials are made available under the
# terms of the Apache License, Version 2.0 which is available at
# https://www.apache.org/licenses/LICENSE-2.0.
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME "vehicle-app-sdk")

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library(${TARGET_NAME}
    sdk/VehicleApp.cpp
    sdk/Model.cpp
    sdk/Node.cpp
    sdk/QueryBuilder.cpp
    sdk/DataPoint.cpp
    sdk/DataPointValue.cpp
    sdk/ThreadPool.cpp
    sdk/Job.cpp
    sdk/Utils.cpp
    sdk/Logger.cpp

    sdk/grpc/GrpcClient.cpp
    sdk/grpc/AsyncGrpcFacade.cpp
    sdk/grpc/BrokerAsyncGrpcFacade.cpp
    sdk/grpc/GrpcDataPointValueProvider.cpp
    sdk/grpc/VehicleDataBrokerClient.cpp

    sdk/middleware/Middleware.cpp
    sdk/middleware/DaprMiddleware.cpp
    sdk/middleware/NativeMiddleware.cpp

    sdk/pubsub/MqttPubSubClient.cpp
    sdk/vdb/DataPointBatch.cpp
)

include_directories(
    ${CMAKE_BINARY_DIR}/gens
    ../include
    .
)

add_dependencies(${TARGET_NAME}
    vehicle-app-sdk-generated-grpc
)

find_package(gRPC REQUIRED)
find_package(cpr REQUIRED)
find_package(fmt REQUIRED)
find_package(PahoMqttCpp REQUIRED)
find_package(nlohmann_json REQUIRED)

target_link_libraries(${TARGET_NAME}
    gRPC::grpc
    cpr::cpr
    fmt::fmt
    PahoMqttCpp::paho-mqttpp3-static
    nlohmann_json::nlohmann_json
    vehicle-app-sdk-generated-grpc
)