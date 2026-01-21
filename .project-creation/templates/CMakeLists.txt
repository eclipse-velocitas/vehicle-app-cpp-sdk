# Copyright (c) 2023 Contributors to the Eclipse Foundation
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

# set the project name
project(VehicleApp CXX)

# APP settings
set(APP_BUILD_TESTS     ON CACHE BOOL "Build the App's tests.")

# Overall settings
set(CMAKE_CXX_STANDARD 17)
set(STATIC_BUILD        OFF CACHE BOOL "Build all targets with external dependencies linked in statically.")

if(STATIC_BUILD)
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()
find_program(CCACHE_FOUND ccache)

if(CCACHE_FOUND)
    message("Found ccache installation")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

# use conan for dependency management
set(CONAN_BUILD_INFO_FILE ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

if(NOT EXISTS ${CONAN_BUILD_INFO_FILE})
    message(FATAL_ERROR "Conan build info file missing. Did you run install_dependencies.sh first?")
    return()
else()
    include(${CONAN_BUILD_INFO_FILE})
    conan_basic_setup(TARGETS)
endif()

add_subdirectory(app)

if(APP_BUILD_TESTS)
    enable_testing()
endif()
