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

cmake_minimum_required(VERSION 3.16)
message("SDK_BUILD_TESTS: ${SDK_BUILD_TESTS}")
message("SDK_BUILD_EXAMPLES: ${SDK_BUILD_EXAMPLES}")
message("STATIC_BUILD: ${STATIC_BUILD}")

set(SDK_BUILD_TESTS     ON CACHE BOOL "Build the SDK tests.")
set(SDK_BUILD_EXAMPLES  ON CACHE BOOL "Build the SDK examples.")

set(STATIC_BUILD        OFF CACHE BOOL "Build all targets with external dependencies linked in statically.")

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/include
)

# set the project name
project(VehicleAppCppSdk CXX)

if(STATIC_BUILD)
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()


find_program(CCACHE_FOUND ccache)

if(CCACHE_FOUND)
    message("Found ccache installation")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()


if(SDK_BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(sdk)

if(SDK_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
