# Copyright (c) 2022 Robert Bosch GmbH
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

set(TARGET_NAME "vehicle-model-example")

add_library(${TARGET_NAME} STATIC
    Vehicle.cpp
    Cabin/Cabin.cpp
    Cabin/Seat/Seat.cpp
    Cabin/SeatService/SeatService.cpp
    Cabin/SeatService/SeatServiceAsyncGrpcFacade.cpp
    service/seats.grpc.pb.cc
    service/seats.pb.cc
)

include_directories(
    ..
    .
)

add_dependencies(${TARGET_NAME}
    vehicle-app-sdk
)

target_link_libraries(${TARGET_NAME}
    vehicle-app-sdk
)
