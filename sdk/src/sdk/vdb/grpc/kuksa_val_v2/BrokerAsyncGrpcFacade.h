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

#ifndef VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_BROKERASYNCGRPCFACADE_H
#define VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_BROKERASYNCGRPCFACADE_H

#include "sdk/grpc/AsyncGrpcFacade.h"
#include "sdk/grpc/GrpcCall.h"

#include "kuksa/val/v2/val.grpc.pb.h"

#include <functional>
#include <memory>

namespace grpc {
class Channel;
class Status;
} // namespace grpc

namespace velocitas::kuksa_val_v2 {

class BrokerAsyncGrpcFacade : public AsyncGrpcFacade {
public:
    explicit BrokerAsyncGrpcFacade(const std::shared_ptr<grpc::Channel>& channel);

    void GetValues(kuksa::val::v2::GetValuesRequest                                    request,
                   std::function<void(const kuksa::val::v2::GetValuesResponse& reply)> replyHandler,
                   std::function<void(const grpc::Status& status)> errorHandler);

    std::shared_ptr<GrpcCall> SubscribeById(
        kuksa::val::v2::SubscribeByIdRequest                                     request,
        std::function<void(const kuksa::val::v2::SubscribeByIdResponse& update)> updateHandler,
        std::function<void(const grpc::Status& status)>                          errorHandler);

    void BatchActuate(
        kuksa::val::v2::BatchActuateRequest                                    request,
        std::function<void(const kuksa::val::v2::BatchActuateResponse& reply)> replyHandler,
        std::function<void(const grpc::Status& status)>                        errorHandler);

    void ListMetadata(
        kuksa::val::v2::ListMetadataRequest                                    request,
        std::function<void(const kuksa::val::v2::ListMetadataResponse& reply)> replyHandler,
        std::function<void(const grpc::Status& status)>                        errorHandler);

private:
    std::unique_ptr<kuksa::val::v2::VAL::StubInterface> m_stub;
};

} // namespace velocitas::kuksa_val_v2

#endif // VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_BROKERASYNCGRPCFACADE_H
