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

set(TARGET_NAME "vehicle-app-sdk-generated-grpc")

add_library(${TARGET_NAME} STATIC
    kuksa/val/v2/types.proto
    kuksa/val/v2/val.proto
    sdv/databroker/v1/types.proto
    sdv/databroker/v1/collector.proto
    sdv/databroker/v1/broker.proto
)

target_include_directories(${TARGET_NAME}
    PUBLIC
    ${absl_INCLUDE_DIRS}
    ${protobuf_INCLUDE_DIRS}
    ${gRPC_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${TARGET_NAME}
    gRPC::grpc++
)

protobuf_generate(TARGET ${TARGET_NAME})
protobuf_generate(
    TARGET ${TARGET_NAME}
    LANGUAGE grpc
    PLUGIN protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
    PLUGIN_OPTIONS generate_mock_code=true
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc)
