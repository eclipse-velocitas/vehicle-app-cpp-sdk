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

#ifndef VEHICLE_APP_SDK_ASYNCGRPCFACADE_H
#define VEHICLE_APP_SDK_ASYNCGRPCFACADE_H

#include <grpcpp/client_context.h>

#include <functional>

namespace velocitas {

class GrpcCall;

class AsyncGrpcFacade {
public:
    using ContextModifierFunction = std::function<void(grpc::ClientContext&)>;

    void setContextModifier(ContextModifierFunction function);

protected:
    void applyContextModifier(GrpcCall& call); // NOLINT

private:
    ContextModifierFunction m_contextModifierFunction;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_ASYNCGRPCFACADE_H
