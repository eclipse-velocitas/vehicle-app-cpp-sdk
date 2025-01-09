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

#ifndef VEHICLE_APP_SDK_GRPCCLIENT_H
#define VEHICLE_APP_SDK_GRPCCLIENT_H

#include <memory>
#include <mutex>
#include <vector>

namespace velocitas {

class GrpcCall;

class GrpcClient {
public:
    GrpcClient()          = default;
    virtual ~GrpcClient() = default;

    GrpcClient(const GrpcClient&)            = delete;
    GrpcClient(GrpcClient&&)                 = delete;
    GrpcClient& operator=(const GrpcClient&) = delete;
    GrpcClient& operator=(GrpcClient&&)      = delete;

    void                 addActiveCall(std::shared_ptr<GrpcCall> call);
    [[nodiscard]] size_t getNumActiveCalls() const { return m_activeCalls.size(); }

private:
    void pruneCompletedRequests();

    std::vector<std::shared_ptr<GrpcCall>> m_activeCalls;
    std::mutex                             m_mutex;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_GRPCCLIENT_H
