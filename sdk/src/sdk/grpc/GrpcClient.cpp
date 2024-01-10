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

#include "sdk/grpc/GrpcClient.h"
#include "sdk/ThreadPool.h"
#include "sdk/grpc/GrpcCall.h"

namespace velocitas {

GrpcClient::GrpcClient() {
    m_recurringJob = std::make_shared<RecurringJob>([this]() { pruneCompletedRequests(); });
    ThreadPool::getInstance()->enqueue(m_recurringJob);
}

GrpcClient::~GrpcClient() {
    m_recurringJob->cancel();
    m_recurringJob->waitForTermination();
}

void GrpcClient::pruneCompletedRequests() {
    static auto isComplete = [](const auto& activeCall) { return activeCall->m_isComplete; };

    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        m_activeCalls.erase(std::remove_if(m_activeCalls.begin(), m_activeCalls.end(), isComplete),
                            m_activeCalls.end());
    }
}

void GrpcClient::addActiveCall(std::shared_ptr<GrpcCall> call) {
    std::scoped_lock<std::mutex> lock(m_mutex);
    m_activeCalls.emplace_back(call);
}

} // namespace velocitas
