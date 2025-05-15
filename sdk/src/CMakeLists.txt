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
    sdk/vdb/grpc/common/ChannelConfiguration.cpp
    sdk/vdb/grpc/common/TypeConversions.cpp
    sdk/vdb/grpc/kuksa_val_v2/BrokerAsyncGrpcFacade.cpp
    sdk/vdb/grpc/kuksa_val_v2/BrokerClient.cpp
    sdk/vdb/grpc/kuksa_val_v2/Metadata.cpp
    sdk/vdb/grpc/kuksa_val_v2/TypeConversions.cpp
    sdk/vdb/grpc/sdv_databroker_v1/BrokerAsyncGrpcFacade.cpp
    sdk/vdb/grpc/sdv_databroker_v1/BrokerClient.cpp
    sdk/vdb/grpc/sdv_databroker_v1/GrpcDataPointValueProvider.cpp
)

target_include_directories(${TARGET_NAME}
    PUBLIC
    ${fmt_INCLUDE_DIRS}
    ${absl_INCLUDE_DIRS}
    ${gRPC_INCLUDE_DIRS}
    ../include
    PRIVATE
    .
)

target_link_libraries(${TARGET_NAME}
    gRPC::grpc++
    fmt::fmt
    PahoMqttCpp::paho-mqttpp3-static
    nlohmann_json::nlohmann_json
    vehicle-app-sdk-generated-grpc
)
