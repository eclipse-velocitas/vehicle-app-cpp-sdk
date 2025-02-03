# Copyright (c) 2022-2025 Contributors to the Eclipse Foundation
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
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(absl REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PahoMqttCpp REQUIRED)
find_package(eclipse-paho-mqtt-c REQUIRED)

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

    sdk/middleware/Middleware.cpp
    sdk/middleware/NativeMiddleware.cpp

    sdk/pubsub/MqttPubSubClient.cpp
    sdk/vdb/DataPointBatch.cpp
    sdk/vdb/IVehicleDataBrokerClient.cpp
    sdk/vdb/grpc/common/TypeConversions.cpp
    sdk/vdb/grpc/kuksa_val_v2/BrokerAsyncGrpcFacade.cpp
    sdk/vdb/grpc/kuksa_val_v2/BrokerClient.cpp
    sdk/vdb/grpc/kuksa_val_v2/TypeConversions.cpp
    sdk/vdb/grpc/sdv_databroker_v1/BrokerAsyncGrpcFacade.cpp
    sdk/vdb/grpc/sdv_databroker_v1/BrokerClient.cpp
    sdk/vdb/grpc/sdv_databroker_v1/GrpcDataPointValueProvider.cpp
)
#message("FMT_INcludedir:${fmt_INCLUDE_DIR}")

include_directories(
    ${CMAKE_BINARY_DIR}/gens
    ../include
    .
    ${fmt_INCLUDE_DIR}
    ${nlohmann_json_INCLUDE_DIR}
    ${gRPC_INCLUDE_DIR}
    ${absl_INCLUDE_DIR}
    ${protobuf_INCLUDE_DIR}
    ${PahoMqttCpp_INCLUDE_DIR}
    ${eclipse-paho-mqtt-c_INCLUDE_DIR}
)

    # find_package(cpr)
    # find_package(fmt)
    # find_package(gRPC)
    # find_package(grpc-proto)
    # find_package(nlohmann_json)
    # find_package(protobuf)
    # find_package(ZLIB)

add_dependencies(${TARGET_NAME}
    vehicle-app-sdk-generated-grpc
)

target_link_libraries(${TARGET_NAME}
    vehicle-app-sdk-generated-grpc
)
