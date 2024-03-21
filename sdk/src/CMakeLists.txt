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

set(TARGET_NAME "vehicle-app-sdk")
find_package(fmt REQUIRED)
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

    # find_package(cpr)
    # find_package(fmt)
    # find_package(gRPC)
    # find_package(grpc-proto)
    # find_package(nlohmann_json)
    # find_package(PahoMqttCpp)
    # find_package(eclipse-paho-mqtt-c)
    # find_package(protobuf)
    # find_package(ZLIB)

add_dependencies(${TARGET_NAME}
    vehicle-app-sdk-generated-grpc
)

target_link_libraries(${TARGET_NAME}
    vehicle-app-sdk-generated-grpc
)
