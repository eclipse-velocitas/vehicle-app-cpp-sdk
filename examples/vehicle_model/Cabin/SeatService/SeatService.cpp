/**
 * Copyright (c) 2022 Robert Bosch GmbH
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

#include "SeatService.h"
#include "sdk/Exceptions.h"
#include "sdk/Logger.h"
#include "sdk/ThreadPool.h"

#include "sdk/dapr/DaprSupport.h"
#include "vehicle_model/Cabin/SeatService/SeatServiceAsyncGrpcFacade.h"

#include <fmt/core.h>
#include <google/protobuf/util/json_util.h>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

namespace velocitas::vehicle::cabin {

sdv::edge::comfort::seats::v1::SeatComponent toGrpcComponent(SeatService::Component component) {
    switch (component) {
    case SeatService::Base:
        return sdv::edge::comfort::seats::v1::SeatComponent::BASE;
    case SeatService::Cushion:
        return sdv::edge::comfort::seats::v1::SeatComponent::CUSHION;
    case SeatService::HeadRestraint:
        return sdv::edge::comfort::seats::v1::SeatComponent::HEAD_RESTRAINT;
    case SeatService::Lumbar:
        return sdv::edge::comfort::seats::v1::SeatComponent::LUMBAR;
    case SeatService::SideBolster:
        return sdv::edge::comfort::seats::v1::SeatComponent::SIDE_BOLSTER;
    default:
        throw InvalidValueException(fmt::format("Invalid component: {}!", component));
    }
}

std::unique_ptr<sdv::edge::comfort::seats::v1::SeatLocation>
toGrpcSeatLocation(const SeatService::SeatLocation& seatLocation) {
    auto grpcSeatLocation = std::make_unique<sdv::edge::comfort::seats::v1::SeatLocation>();
    grpcSeatLocation->set_index(seatLocation.index);
    grpcSeatLocation->set_row(seatLocation.row);
    return grpcSeatLocation;
}

std::unique_ptr<sdv::edge::comfort::seats::v1::Position>
toGrpcSeatPosition(const SeatService::Positions& positions) {
    auto grpcSeatPositions = std::make_unique<sdv::edge::comfort::seats::v1::Position>();
    grpcSeatPositions->set_base(positions.base);
    grpcSeatPositions->set_cushion(positions.cushion);
    grpcSeatPositions->set_head_restraint(positions.headRestraint);
    grpcSeatPositions->set_lumbar(positions.lumbar);
    grpcSeatPositions->set_side_bolster(positions.sideBolster);
    return grpcSeatPositions;
}

std::unique_ptr<sdv::edge::comfort::seats::v1::Seat> toGrpcSeat(const SeatService::Seat& seat) {
    auto grpcSeat = std::make_unique<sdv::edge::comfort::seats::v1::Seat>();
    grpcSeat->set_allocated_location(toGrpcSeatLocation(seat.location).release()); // NOLINT
    grpcSeat->set_allocated_position(toGrpcSeatPosition(seat.position).release()); // NOLINT
    return grpcSeat;                                                               // NOLINT
}

Status toInternalStatus(grpc::Status status) {
    if (status.ok()) {
        return {};
    }

    return {status.error_message()};
}

SeatService::SeatService(Model* parent)
    : Service(m_appID, parent) {
    dapr::waitForSidecar();

    m_asyncGrpcFacade = std::make_shared<SeatServiceAsyncGrpcFacade>(
        grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    m_asyncGrpcFacade->setContextModifier(
        [this](auto& context) { context.AddMetadata("dapr-app-id", m_appID); });
}

AsyncResultPtr_t<VoidResult> SeatService::move(const SeatService::Seat& seat) {
    auto asyncResult = std::make_shared<AsyncResult<VoidResult>>();

    m_asyncGrpcFacade->Move(
        toGrpcSeat(seat),
        [asyncResult](const auto& reply) { asyncResult->insertResult(VoidResult{}); },
        [asyncResult](const auto& status) { asyncResult->insertError(toInternalStatus(status)); });

    return asyncResult;
}

AsyncResultPtr_t<VoidResult>
SeatService::moveComponent(const SeatService::SeatLocation&       seatLocation,
                           vehicle::cabin::SeatService::Component component, int position) {
    auto asyncResult = std::make_shared<AsyncResult<VoidResult>>();

    m_asyncGrpcFacade->MoveComponent(
        toGrpcSeatLocation(seatLocation), toGrpcComponent(component), position,
        [asyncResult](const auto& reply) { asyncResult->insertResult(VoidResult{}); },
        [asyncResult](const auto& status) { asyncResult->insertError(toInternalStatus(status)); });

    return asyncResult;
}

AsyncResultPtr_t<std::string> SeatService::getCurrentPosition(int row, int index) {
    auto asyncResult = std::make_shared<AsyncResult<std::string>>();

    m_asyncGrpcFacade->CurrentPosition(
        row, index,
        [asyncResult](const auto& reply) {
            std::string output;
            google::protobuf::util::MessageToJsonString(reply, &output);
            asyncResult->insertResult(std::move(output));
        },
        [asyncResult](const auto& status) { asyncResult->insertError(toInternalStatus(status)); });

    return asyncResult;
}

} // namespace velocitas::vehicle::cabin
