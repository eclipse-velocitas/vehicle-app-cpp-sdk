# Copyright (c) 2023-2025 Contributors to the Eclipse Foundation
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

set(TARGET_NAME "sdk_utests")

add_executable(${TARGET_NAME}
    testmain.cpp
    AsyncResult_tests.cpp
    AsyncSubscription_tests.cpp
    DataPoint_tests.cpp
    DataPointBatch_tests.cpp
    DataPointValue_tests.cpp
    Job_tests.cpp
    Logger_tests.cpp
    Middleware_tests.cpp
    NativeMiddleware_tests.cpp
    Node_tests.cpp
    ScopedBoolInverter_tests.cpp
    ThreadPool_tests.cpp
    Utils_tests.cpp
    QueryBuilder_tests.cpp
    #PubSub_tests.cpp
    TestBaseUsingEnvVars.cpp
    grpc/GrpcClient_tests.cpp
    vdb/grpc/kuksa_val_v2/TypeConversions_tests.cpp
    vdb/grpc/sdv_databroker_v1/BrokerClient_tests.cpp
)

target_link_libraries(${TARGET_NAME}
    vehicle-app-sdk
    gmock
)

include(GoogleTest)

target_include_directories(${TARGET_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/../model
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src
)

if(NOT CMAKE_TOOLCHAIN_FILE)
    gtest_discover_tests(${TARGET_NAME})
endif()
