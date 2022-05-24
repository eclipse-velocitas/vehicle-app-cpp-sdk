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

#include "vehicle_model/Cabin/SeatService/SeatServiceAsyncGrpcFacade.h"
#include "sdk/grpc/GrpcCall.h"

#include "grpcpp/channel.h"

namespace velocitas {

SeatServiceAsyncGrpcFacade::SeatServiceAsyncGrpcFacade(std::shared_ptr<grpc::Channel> channel)
    : m_channel{std::move(channel)} {
    m_stub = std::make_shared<sdv::edge::comfort::seats::v1::Seats::Stub>(m_channel);
}

void SeatServiceAsyncGrpcFacade::Move(
    std::unique_ptr<sdv::edge::comfort::seats::v1::Seat>&&                     seat,
    std::function<void(const sdv::edge::comfort::seats::v1::MoveReply& reply)> replyHandler,
    std::function<void(const grpc::Status& status)>                            errorHandler) {
    auto callData =
        std::make_shared<GrpcSingleResponseCall<sdv::edge::comfort::seats::v1::MoveRequest,
                                                sdv::edge::comfort::seats::v1::MoveReply>>();

    applyContextModifier(*callData);

    callData->m_request.set_allocated_seat(seat.release());

    auto grpcResultHandler = [callData, replyHandler, errorHandler](grpc::Status status) {
        if (status.ok()) {
            replyHandler(callData->m_reply);
        } else {
            errorHandler(status);
        };
    };

    addActiveCall(callData);

    m_stub->async()->Move(&callData->m_context, &callData->m_request, &callData->m_reply,
                          grpcResultHandler);
}

void SeatServiceAsyncGrpcFacade::MoveComponent(
    std::unique_ptr<sdv::edge::comfort::seats::v1::SeatLocation>&& location,
    sdv::edge::comfort::seats::v1::SeatComponent seatComponent, int position,
    std::function<void(const sdv::edge::comfort::seats::v1::MoveComponentReply& reply)>
                                                    replyHandler,
    std::function<void(const grpc::Status& status)> errorHandler) {
    auto callData = std::make_shared<
        GrpcSingleResponseCall<sdv::edge::comfort::seats::v1::MoveComponentRequest,
                               sdv::edge::comfort::seats::v1::MoveComponentReply>>();

    applyContextModifier(*callData);

    callData->m_request.set_allocated_seat(location.release());
    callData->m_request.set_component(seatComponent);
    callData->m_request.set_position(position);

    auto grpcResultHandler = [callData, replyHandler, errorHandler](grpc::Status status) {
        if (status.ok()) {
            replyHandler(callData->m_reply);
        } else {
            errorHandler(status);
        };
    };

    addActiveCall(callData);

    m_stub->async()->MoveComponent(&callData->m_context, &callData->m_request, &callData->m_reply,
                                   grpcResultHandler);
}

void SeatServiceAsyncGrpcFacade::CurrentPosition(
    int row, int index,
    std::function<void(const sdv::edge::comfort::seats::v1::CurrentPositionReply& reply)>
                                                    replyHandler,
    std::function<void(const grpc::Status& status)> errorHandler) {
    auto callData = std::make_shared<
        GrpcSingleResponseCall<sdv::edge::comfort::seats::v1::CurrentPositionRequest,
                               sdv::edge::comfort::seats::v1::CurrentPositionReply>>();

    applyContextModifier(*callData);

    callData->m_request.set_row(row);
    callData->m_request.set_index(index);

    auto grpcResultHandler = [callData, replyHandler, errorHandler](grpc::Status status) {
        if (status.ok()) {
            replyHandler(callData->m_reply);
        } else {
            errorHandler(status);
        };
    };

    addActiveCall(callData);

    m_stub->async()->CurrentPosition(&callData->m_context, &callData->m_request, &callData->m_reply,
                                     grpcResultHandler);
}

} // namespace velocitas
