/**
 * Copyright (c) 2022-2023 Contributors to the Eclipse Foundation
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

#ifndef VEHICLE_APP_SDK_SEATSERVICEASYNCGRPCFACADE_H
#define VEHICLE_APP_SDK_SEATSERVICEASYNCGRPCFACADE_H

#include "sdk/grpc/AsyncGrpcFacade.h"
#include "sdk/grpc/GrpcClient.h"

#include "service/seats.grpc.pb.h"

#include <memory>

namespace grpc {
class Channel;
}

namespace velocitas {

class SeatServiceAsyncGrpcFacade : public AsyncGrpcFacade, GrpcClient {
public:
    explicit SeatServiceAsyncGrpcFacade(std::shared_ptr<grpc::Channel> channel);

    void
    Move(std::unique_ptr<sdv::edge::comfort::seats::v1::Seat>&&                     seat,
         std::function<void(const sdv::edge::comfort::seats::v1::MoveReply& reply)> replyHandler,
         std::function<void(const grpc::Status& status)>                            errorHandler);

    void MoveComponent(
        std::unique_ptr<sdv::edge::comfort::seats::v1::SeatLocation>&& location,
        sdv::edge::comfort::seats::v1::SeatComponent seatComponent, int position,
        std::function<void(const sdv::edge::comfort::seats::v1::MoveComponentReply& reply)>
                                                        replyHandler,
        std::function<void(const grpc::Status& status)> errorHandler);

    void CurrentPosition(
        int row, int index,
        std::function<void(const sdv::edge::comfort::seats::v1::CurrentPositionReply& reply)>
                                                        replyHandler,
        std::function<void(const grpc::Status& status)> errorHandler);

private:
    std::shared_ptr<sdv::edge::comfort::seats::v1::Seats::Stub> m_stub;
    std::shared_ptr<grpc::Channel>                              m_channel;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_SEATSERVICEASYNCGRPCFACADE_H
