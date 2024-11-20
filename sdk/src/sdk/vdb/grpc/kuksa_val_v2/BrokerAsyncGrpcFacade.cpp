/**
 * Copyright (c) 2022-2024 Contributors to the Eclipse Foundation
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

#include "BrokerAsyncGrpcFacade.h"

#include "sdk/Logger.h"
#include "sdk/grpc/GrpcCall.h"

#include <grpcpp/channel.h>

namespace velocitas::kuksa_val_v2 {

BrokerAsyncGrpcFacade::BrokerAsyncGrpcFacade(const std::shared_ptr<grpc::Channel>& channel)
    : m_stub{kuksa::val::v2::VAL::NewStub(channel)} {}

void BrokerAsyncGrpcFacade::GetValues(
    kuksa::val::v2::GetValuesRequest                                    request,
    std::function<void(const kuksa::val::v2::GetValuesResponse& reply)> replyHandler,
    std::function<void(const grpc::Status& status)>                     errorHandler) {
    auto callData = std::make_shared<GrpcSingleResponseCall<kuksa::val::v2::GetValuesRequest,
                                                            kuksa::val::v2::GetValuesResponse>>();

    callData->m_request = std::move(request);

    applyContextModifier(*callData);

    auto grpcResultHandler = [callData, replyHandler, errorHandler](grpc::Status status) {
        try {
            if (status.ok()) {
                replyHandler(callData->m_reply);
            } else {
                errorHandler(status);
            };
        } catch (std::exception& e) {
            logger().error("GRPC: Exception occurred during \"GetDatapoints\": {}", e.what());
        }
        callData->m_isComplete = true;
    };

    addActiveCall(callData);

    m_stub->async()->GetValues(&callData->m_context, &callData->m_request, &callData->m_reply,
                               grpcResultHandler);
}

void BrokerAsyncGrpcFacade::BatchActuate(
    kuksa::val::v2::BatchActuateRequest                                    request,
    std::function<void(const kuksa::val::v2::BatchActuateResponse& reply)> replyHandler,
    std::function<void(const grpc::Status& status)>                        errorHandler) {
    auto callData =
        std::make_shared<GrpcSingleResponseCall<kuksa::val::v2::BatchActuateRequest,
                                                kuksa::val::v2::BatchActuateResponse>>();

    callData->m_request = std::move(request);

    applyContextModifier(*callData);

    auto grpcResultHandler = [callData, replyHandler, errorHandler](grpc::Status status) {
        try {
            if (status.ok()) {
                replyHandler(callData->m_reply);
            } else {
                errorHandler(status);
            };
        } catch (std::exception& e) {
            logger().error("GRPC: Exception occurred during \"SetDatapoints\": {}", e.what());
        }
        callData->m_isComplete = true;
    };

    addActiveCall(callData);

    m_stub->async()->BatchActuate(&callData->m_context, &callData->m_request, &callData->m_reply,
                                  grpcResultHandler);
}

void BrokerAsyncGrpcFacade::Subscribe(
    kuksa::val::v2::SubscribeRequest                                    request,
    std::function<void(const kuksa::val::v2::SubscribeResponse& reply)> itemHandler,
    std::function<void(const grpc::Status& status)>                     errorHandler) {
    auto callData =
        std::make_shared<GrpcStreamingResponseCall<kuksa::val::v2::SubscribeRequest,
                                                   kuksa::val::v2::SubscribeResponse>>();

    callData->getRequest() = std::move(request);

    applyContextModifier(*callData);

    addActiveCall(callData);

    m_stub->async()->Subscribe(&callData->m_context, &callData->getRequest(),
                               &callData->getReactor());

    callData->onData(itemHandler);

    callData->onFinish([callData, errorHandler](const auto& status) {
        if (!status.ok()) {
            errorHandler(status);
        }
        callData->m_isComplete = true;
    });

    callData->startCall();
}

} // namespace velocitas::kuksa_val_v2
