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
 * @brief A GRPC call where a request is followed up by a single response.
 *
 * @tparam TRequestType   The data type of the request.
 * @tparam TResponseType  The data type of the (success) response.
 */
template <class TRequestType, class TResponseType> class GrpcSingleResponseCall : public GrpcCall {
public:
    GrpcSingleResponseCall() = default;
    explicit GrpcSingleResponseCall(TRequestType request)
        : m_request(std::move(request)) {}
    TRequestType  m_request;
    TResponseType m_response;
};

/**
 * @brief A GRPC call where a request is followed up by multiple streamed responses.
 *
 * @tparam TRequestType   The data type of the request.
 * @tparam TResponseType  The data type of a single response.
 */
template <class TRequestType, class TResponseType>
class GrpcStreamingResponseCall : public GrpcCall, private grpc::ClientReadReactor<TResponseType> {
public:
    GrpcStreamingResponseCall() = default;
    explicit GrpcStreamingResponseCall(TRequestType request)
        : m_request(std::move(request)) {}

    GrpcStreamingResponseCall& startCall() {
        this->StartRead(&m_response);
        this->StartCall();
        return *this;
    }

    GrpcStreamingResponseCall& onData(std::function<void(const TResponseType&)> handler) {
        m_onResponseHandler = handler;
        return *this;
    }

    GrpcStreamingResponseCall& onFinish(std::function<void(const grpc::Status&)> handler) {
        m_onFinishHandler = handler;
        return *this;
    }

    TRequestType& getRequest() { return m_request; }

    grpc::ClientReadReactor<TResponseType>& getReactor() { return *this; }

private:
    void OnReadDone(bool isOk) override {
        if (isOk) {
            try {
                m_onResponseHandler(m_response);
            } catch (const std::exception& e) {
                velocitas::logger().error(
                    "GrpcCall: Exception occurred during response handler notification: {}",
                    e.what());
            }
            this->StartRead(&m_response);
        }
    }

    void OnDone(const grpc::Status& status) override {
        m_onFinishHandler(status);
        m_isComplete = true;
    }

    TRequestType                              m_request;
    TResponseType                             m_response;
    std::function<void(const TResponseType&)> m_onResponseHandler;
    std::function<void(const grpc::Status&)>  m_onFinishHandler;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_GRPCCALL_H
