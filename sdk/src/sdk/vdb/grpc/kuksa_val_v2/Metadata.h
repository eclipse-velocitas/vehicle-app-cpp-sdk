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

#ifndef VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_METADATA_H
#define VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_METADATA_H

#include <grpcpp/impl/status.h>

#include <cstdint>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <shared_mutex>
#include <string>
#include <vector>

namespace grpc {
class Status;
}

namespace velocitas::kuksa_val_v2 {

class BrokerAsyncGrpcFacade;

struct Metadata {
    // enum class State {
    //     NOT_YET_GAINED,
    //     AVAILABLE,
    //     UNKNOWN_SIGNAL,
    //     PERMISSION_DENIED,
    //     INCOMPATIBLE,
    // };

    // State       m_state{State::NOT_YET_GAINED};
    std::string m_path;
    int32_t     m_id{0};
    bool        m_isKnown{false};
};

class MetadataRequester;
struct Query;

/**
 * Provides the Graph API to access vehicle signals via the kuksa.val.v2 API
 */
class MetadataCache {
public:
    explicit MetadataCache(const std::shared_ptr<BrokerAsyncGrpcFacade>& asyncBrokerFacade);

    void query(const std::vector<std::string>& signalPaths, std::function<void()> onSuccess,
               std::function<void(const grpc::Status&)> onError);
    void invalidate(grpc::StatusCode statusCode = grpc::StatusCode::UNAVAILABLE);

    std::shared_ptr<Metadata> getById(int32_t id) const;
    std::shared_ptr<Metadata> getByPath(const std::string& path) const;

private:
    void onMetadata(const std::shared_ptr<Metadata>& metadata);
    void onError(const std::string& path, grpc::Status&& status);
    bool isPresent(const std::string& path) const;
    void addQuery(Query&& query);
    std::deque<std::string>
                      determineMissingSignals(const std::vector<std::string>& signalPaths) const;
    std::deque<Query> withdrawFulfilledQueries(const std::string& path);
    std::deque<Query> withdrawAffectedQueries(const std::string& path);
    void              notifyQueryInitiators(std::deque<Query>&& fulfilledQueries) const;
    void notifyQueryInitiators(std::deque<Query>&& affectedQueries, grpc::Status&& status) const;

    const std::unique_ptr<MetadataRequester> m_metadataRequester;

    mutable std::shared_mutex                        m_mutex;
    std::map<int32_t, std::shared_ptr<Metadata>>     m_idMap;
    std::map<std::string, std::shared_ptr<Metadata>> m_pathMap;
    std::deque<Query>                                m_pendingQueries;
};

} // namespace velocitas::kuksa_val_v2

#endif // VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_METADATA_H
