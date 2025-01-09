/**
 * Copyright (c) 2024-2025 Contributors to the Eclipse Foundation
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "SeatsServiceImpl.h"
#include <sdk/VehicleApp.h>
#include <sdk/VehicleModelContext.h>
#include <sdk/vdb/IVehicleDataBrokerClient.h>

namespace velocitas {

::grpc::Status SeatsService::Move(::grpc::ServerContext*                              context,
                                  const ::sdv::edge::comfort::seats::v1::MoveRequest* request,
                                  ::sdv::edge::comfort::seats::v1::MoveReply*         response) {
    (void)context;
    (void)request;
    (void)response;
    auto seat     = request->seat();
    auto location = seat.location();
    auto row      = location.row();
    auto pos      = location.index();

    std::cout << "Got Move Request!" << std::endl;

    try {
        auto status =
            vehicle.Cabin.Seat.Row1.DriverSide.Position.set(seat.position().base())->await();

        if (status.ok()) {
            std::cout << "OK!" << std::endl;
            return ::grpc::Status(::grpc::StatusCode::OK, "");
        } else {
            std::cout << "Some error!" << std::endl;
            // This could for instance happen if datapoint is not known by databroker
            // then message will be UNKNOWN_DATAPOINT.
            return ::grpc::Status(::grpc::StatusCode::CANCELLED, status.errorMessage());
        }
    } catch (AsyncException& e) {
        std::cout << "Async Exception!" << std::endl;
        // This could typically be that Databroker is not running or not reachable
        // or that your Velocitas environment uses an API incompatible with what your Databroker
        // instance supports
        return ::grpc::Status(::grpc::StatusCode::UNAVAILABLE, "");
    }
}

::grpc::Status
SeatsService::MoveComponent(::grpc::ServerContext*                                       context,
                            const ::sdv::edge::comfort::seats::v1::MoveComponentRequest* request,
                            ::sdv::edge::comfort::seats::v1::MoveComponentReply*         response) {
    (void)context;
    (void)request;
    (void)response;
    // This is an example of an unimplemented method
    return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status SeatsService::CurrentPosition(
    ::grpc::ServerContext*                                         context,
    const ::sdv::edge::comfort::seats::v1::CurrentPositionRequest* request,
    ::sdv::edge::comfort::seats::v1::CurrentPositionReply*         response) {
    (void)context;
    (void)request;

    std::cout << "Got CurrentPosition Request!" << std::endl;

    auto seat          = response->mutable_seat();
    auto seat_position = seat->mutable_position();
    try {
        auto seatPos = vehicle.Cabin.Seat.Row1.DriverSide.Position.get()->await().value();

        std::cout << "Success!!" << std::endl;
        seat_position->set_base(seatPos);
        return ::grpc::Status(::grpc::StatusCode::OK, "");
    } catch (AsyncException& e) {
        std::cout << "Async Exception!" << std::endl;
        // This could typically be that Databroker is not running or not reachable
        // or that your Velocitas environment uses an API incompatible with what your Databroker
        // instance supports
        return ::grpc::Status(::grpc::StatusCode::OK, "");
    } catch (InvalidValueException& e) {
        std::cout << "Invalid Value!" << std::endl;
        // This could be given if Databroker has no current value for Position
        // (Have you set it manually using kuksa-client or databroker-cli?)
        return ::grpc::Status(::grpc::StatusCode::OK, "");
    }
}

} // namespace velocitas
