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

#find_package(fmt CONFIG REQUIRED)
#find_package(nlohmann_json CONFIG REQUIRED)

include_directories(
    ${fmt_INCLUDE_DIR}
    ${nlohmann_json_INCLUDE_DIR}
    example_model
)

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
message(STATUS "------------------------------------------------------------------------------------------------------")
message(STATUS "${CMAKE_CURRENT_LIST_DIR}")
message(STATUS "------------------------------------------------------------------------------------------------------")
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
message(STATUS "------------------------------------------------------------------------------------------------------")

# Currently only adding examples that can be tested without additional velocitas code generation

add_subdirectory(performance-subscribe)
add_subdirectory(seat-adjuster)
add_subdirectory(set-data-points)
