/**
 * Copyright (c) 2022-2025 Contributors to the Eclipse Foundation
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
    kuksa::val::v2::GetValuesRequest                                       request,
    std::function<void(const kuksa::val::v2::GetValuesResponse& response)> responseHandler,
    std::function<void(const grpc::Status& status)>                        errorHandler) {
    auto callData = std::make_shared<GrpcSingleResponseCall<kuksa::val::v2::GetValuesRequest,
                                                            kuksa::val::v2::GetValuesResponse>>(
        std::move(request));
    applyContextModifier(*callData);

    auto grpcResultHandler = [callData, responseHandler, errorHandler](grpc::Status status) {
        try {
            if (status.ok()) {
                responseHandler(callData->m_response);
            } else {
                errorHandler(status);
            };
        } catch (std::exception& e) {
            logger().error("GRPC: Exception occurred during \"GetValues\": {}", e.what());
        }
        callData->m_isComplete = true;
    };

    m_stub->async()->GetValues(&callData->m_context, &callData->m_request, &callData->m_response,
                               grpcResultHandler);
}

void BrokerAsyncGrpcFacade::BatchActuate(
    kuksa::val::v2::BatchActuateRequest                                       request,
    std::function<void(const kuksa::val::v2::BatchActuateResponse& response)> responseHandler,
    std::function<void(const grpc::Status& status)>                           errorHandler) {
    auto callData = std::make_shared<GrpcSingleResponseCall<kuksa::val::v2::BatchActuateRequest,
                                                            kuksa::val::v2::BatchActuateResponse>>(
        std::move(request));
    applyContextModifier(*callData);

    auto grpcResultHandler = [callData, responseHandler, errorHandler](grpc::Status status) {
        try {
            if (status.ok()) {
                responseHandler(callData->m_response);
            } else {
                errorHandler(status);
            };
        } catch (std::exception& e) {
            logger().error("GRPC: Exception occurred during \"BatchActuate\": {}", e.what());
        }
        callData->m_isComplete = true;
    };

    m_stub->async()->BatchActuate(&callData->m_context, &callData->m_request, &callData->m_response,
                                  grpcResultHandler);
}

std::shared_ptr<GrpcCall> BrokerAsyncGrpcFacade::SubscribeById(
    kuksa::val::v2::SubscribeByIdRequest                                       request,
    std::function<void(const kuksa::val::v2::SubscribeByIdResponse& response)> updateHandler,
    std::function<void(const grpc::Status& status)>                            finishHandler) {
    auto callData =
        std::make_shared<GrpcStreamingResponseCall<kuksa::val::v2::SubscribeByIdRequest,
                                                   kuksa::val::v2::SubscribeByIdResponse>>(
            std::move(request));
    applyContextModifier(*callData);

    m_stub->async()->SubscribeById(&callData->m_context, &callData->getRequest(),
                                   &callData->getReactor());

    callData->onData(updateHandler);
    callData->onFinish(finishHandler);
    callData->startCall();
    return callData;
}

void BrokerAsyncGrpcFacade::ListMetadata(
    kuksa::val::v2::ListMetadataRequest                                       request,
    std::function<void(const kuksa::val::v2::ListMetadataResponse& response)> responseHandler,
    std::function<void(const grpc::Status& status)>                           errorHandler) {
    auto callData = std::make_shared<GrpcSingleResponseCall<kuksa::val::v2::ListMetadataRequest,
                                                            kuksa::val::v2::ListMetadataResponse>>(
        std::move(request));
    applyContextModifier(*callData);

    auto grpcResultHandler = [callData, responseHandler, errorHandler](grpc::Status status) {
        try {
            if (status.ok()) {
                responseHandler(callData->m_response);
            } else {
                errorHandler(status);
            };
        } catch (std::exception& e) {
            logger().error("GRPC: Exception occurred during \"ListMetadata\": {}", e.what());
        }
        callData->m_isComplete = true;
    };

    m_stub->async()->ListMetadata(&callData->m_context, &callData->m_request, &callData->m_response,
                                  grpcResultHandler);
}

} // namespace velocitas::kuksa_val_v2
