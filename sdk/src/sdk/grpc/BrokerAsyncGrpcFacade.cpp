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

#include "sdk/grpc/BrokerAsyncGrpcFacade.h"
#include "sdk/Logger.h"
#include "sdk/grpc/GrpcCall.h"

#include <grpcpp/channel.h>

namespace velocitas {

BrokerAsyncGrpcFacade::BrokerAsyncGrpcFacade(std::shared_ptr<grpc::Channel> channel)
    : m_channel{std::move(channel)} {
    m_stub = std::make_shared<sdv::databroker::v1::Broker::Stub>(m_channel);
}

void BrokerAsyncGrpcFacade::GetDatapoints(
    const std::vector<std::string>&                                           datapoints,
    std::function<void(const sdv::databroker::v1::GetDatapointsReply& reply)> replyHandler,
    std::function<void(const grpc::Status& status)>                           errorHandler) {
    auto callData =
        std::make_shared<GrpcSingleResponseCall<sdv::databroker::v1::GetDatapointsRequest,
                                                sdv::databroker::v1::GetDatapointsReply>>();

    std::for_each(datapoints.begin(), datapoints.end(), [&callData](const auto& dataPoint) {
        callData->m_request.add_datapoints(dataPoint);
    });

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
    };

    addActiveCall(callData);

    m_stub->async()->GetDatapoints(&callData->m_context, &callData->m_request, &callData->m_reply,
                                   grpcResultHandler);
}

void BrokerAsyncGrpcFacade::SetDatapoints(
    const std::map<std::string, sdv::databroker::v1::Datapoint>&              datapoints,
    std::function<void(const sdv::databroker::v1::SetDatapointsReply& reply)> replyHandler,
    std::function<void(const grpc::Status& status)>                           errorHandler) {
    auto callData =
        std::make_shared<GrpcSingleResponseCall<sdv::databroker::v1::SetDatapointsRequest,
                                                sdv::databroker::v1::SetDatapointsReply>>();

    for (const auto [key, value] : datapoints) {
        (*callData->m_request.mutable_datapoints())[key] = value;
    }

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
    };

    addActiveCall(callData);

    m_stub->async()->SetDatapoints(&callData->m_context, &callData->m_request, &callData->m_reply,
                                   grpcResultHandler);
}

void BrokerAsyncGrpcFacade::Subscribe(
    const std::string&                                                    query,
    std::function<void(const sdv::databroker::v1::SubscribeReply& reply)> itemHandler,
    std::function<void(const grpc::Status& status)>                       errorHandler) {
    auto callData =
        std::make_shared<GrpcStreamingResponseCall<sdv::databroker::v1::SubscribeRequest,
                                                   sdv::databroker::v1::SubscribeReply>>();

    callData->getRequest().set_query(query);

    applyContextModifier(*callData);

    addActiveCall(callData);

    m_stub->async()->Subscribe(&callData->m_context, &callData->getRequest(),
                               &callData->getReactor());

    callData->onData(itemHandler);

    callData->onFinish([errorHandler](const auto& status) {
        if (!status.ok()) {
            errorHandler(status);
        }
    });

    callData->startCall();
}

} // namespace velocitas
