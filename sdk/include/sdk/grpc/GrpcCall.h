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

#ifndef VEHICLE_APP_SDK_GRPCCALL_H
#define VEHICLE_APP_SDK_GRPCCALL_H

#include "sdk/Logger.h"

#include <fmt/core.h>
#include <functional>
#include <grpcpp/client_context.h>
#include <grpcpp/impl/codegen/client_callback.h>

namespace velocitas {

/**
 * @brief Base class for implementing GRPC calls.
 *
 */
class GrpcCall {
public:
    grpc::ClientContext m_context;
    bool                m_isComplete{false};
};

/**
 * @brief A GRPC call where a request is followed up by a single reply.
 *
 * @tparam TRequestType The data type of the request.
 * @tparam TReplyType   The data type of the reply.
 */
template <class TRequestType, class TReplyType> class GrpcSingleResponseCall : public GrpcCall {
public:
    TRequestType m_request;
    TReplyType   m_reply;
};

/**
 * @brief A GRPC call where a request is followed up by multiple streamed replies.
 *
 * @tparam TRequestType The data type of the request.
 * @tparam TReplyType   The data type of the reply.
 */
template <class TRequestType, class TReplyType> class GrpcStreamingResponseCall : public GrpcCall {
private:
    class ReadReactor : public grpc::ClientReadReactor<TReplyType> {
    public:
        explicit ReadReactor(GrpcStreamingResponseCall& parent) // NOLINT
            : m_parent(parent) {}

        void OnReadDone(bool isOk) override {
            if (isOk) {
                try {
                    m_onDataHandler(m_reply);
                } catch (std::exception& e) {
                    velocitas::logger().error(
                        "GRPC: Exception occurred during \"GetDatapoints\": {}", e.what());
                }
                this->StartRead(&m_reply);
            }
        }

        void OnDone(const grpc::Status& status) override {
            m_onFinishHandler(status);
            m_parent.m_isComplete = true;
        }

    private:
        TReplyType                               m_reply;
        std::function<void(const TReplyType&)>   m_onDataHandler;
        std::function<void(const grpc::Status&)> m_onFinishHandler;
        GrpcStreamingResponseCall&               m_parent;

        friend class GrpcStreamingResponseCall;
    };

public:
    GrpcStreamingResponseCall()
        : m_readReactor(std::make_shared<ReadReactor>(*this)) {}

    GrpcStreamingResponseCall& startCall() {
        m_readReactor->StartCall();
        m_readReactor->StartRead(&m_readReactor->m_reply);
        return *this;
    }

    GrpcStreamingResponseCall& onData(std::function<void(const TReplyType&)> handler) {
        m_readReactor->m_onDataHandler = handler;
        return *this;
    }

    GrpcStreamingResponseCall& onFinish(std::function<void(const grpc::Status&)> handler) {
        m_readReactor->m_onFinishHandler = handler;
        return *this;
    }

    TRequestType& getRequest() { return m_request; }

    ReadReactor& getReactor() { return *(m_readReactor.get()); }

private:
    TRequestType                 m_request;
    std::shared_ptr<ReadReactor> m_readReactor;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_GRPCCALL_H
