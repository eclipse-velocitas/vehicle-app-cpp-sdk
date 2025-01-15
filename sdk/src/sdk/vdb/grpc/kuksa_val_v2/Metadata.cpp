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

#include "Metadata.h"

#include "sdk/Logger.h"
#include "sdk/vdb/grpc/kuksa_val_v2/BrokerAsyncGrpcFacade.h"

#include <algorithm>
#include <deque>
#include <limits>
#include <mutex>
#include <set>
#include <stdexcept>
#include <utility>

namespace velocitas::kuksa_val_v2 {

namespace {

const unsigned int MAX_PARALLEL_REQUESTS{5};

} // namespace

class MetadataRequester {
public:
    MetadataRequester(std::shared_ptr<BrokerAsyncGrpcFacade>                  asyncBrokerFacade,
                      std::function<void(std::shared_ptr<Metadata>&&)>        metadataCallback,
                      std::function<void(const std::string&, grpc::Status&&)> errorCallback)
        : m_asyncBrokerFacade(std::move(asyncBrokerFacade))
        , m_metadataCallback(std::move(metadataCallback))
        , m_errorCallback(std::move(errorCallback)) {}

    void addSignalsToRequest(std::deque<std::string>&& signalsToRequest) {
        {
            std::unique_lock lock(m_mutex);
            while (!signalsToRequest.empty()) {
                addSignalToRequest(std::move(signalsToRequest.front()));
                signalsToRequest.pop_front();
            }
        }
        checkToRequestMetadata();
    }

    void reset() {
        std::unique_lock lock(m_mutex);
        m_pendingSignals.clear();
    }

private:
    void addSignalToRequest(std::string&& path) {
        if ((m_activeRequests.count(path) == 0) &&
            (std::find(m_pendingSignals.cbegin(), m_pendingSignals.cend(), path) ==
             m_pendingSignals.end())) {
            m_pendingSignals.push_back(std::move(path));
        }
    }

    void checkToRequestMetadata() {
        std::string path;
        {
            std::unique_lock lock(m_mutex);
            if (!m_pendingSignals.empty() && (m_activeRequests.size() < MAX_PARALLEL_REQUESTS)) {
                path = std::move(m_pendingSignals.front());
                m_pendingSignals.pop_front();
                m_activeRequests.insert(path);
            }
        }
        if (!path.empty()) {
            requestMetadata(path);
        }
    }

    void requestMetadata(const std::string& path) {
        kuksa::val::v2::ListMetadataRequest request;
        request.set_root(path);
        m_asyncBrokerFacade->ListMetadata(
            std::move(request),
            [this, path](auto&& response) {
                {
                    std::unique_lock lock(m_mutex);
                    m_activeRequests.erase(path);
                }
                checkToRequestMetadata();

                if (response.metadata_size() != 0) {
                    m_metadataCallback(std::make_shared<Metadata>(
                        Metadata{path, response.metadata(0).id(), true}));
                } else {
                    logger().warn(
                        "Databroker returned empty metadata list for {} -> assuming as 'unknown'",
                        path);
                    m_metadataCallback(std::make_shared<Metadata>(Metadata{path, 0, false}));
                }
            },
            [this, path](auto status) {
                if (status.error_code() == grpc::StatusCode::DEADLINE_EXCEEDED) {
                    // retry getting that metadata
                    requestMetadata(path);
                    return;
                }
                {
                    std::unique_lock lock(m_mutex);
                    m_activeRequests.erase(path);
                }
                checkToRequestMetadata();
                if (status.error_code() == grpc::StatusCode::NOT_FOUND ||
                    status.error_code() == grpc::StatusCode::PERMISSION_DENIED) {
                    m_metadataCallback(std::make_shared<Metadata>(Metadata{path, 0, false}));
                } else if (status.error_code() != grpc::StatusCode::DEADLINE_EXCEEDED) {
                    m_errorCallback(path, std::move(status));
                }
            });
    }

    std::shared_ptr<BrokerAsyncGrpcFacade>                  m_asyncBrokerFacade;
    std::function<void(std::shared_ptr<Metadata>&&)>        m_metadataCallback;
    std::function<void(const std::string&, grpc::Status&&)> m_errorCallback;

    std::mutex              m_mutex;
    std::deque<std::string> m_pendingSignals;
    std::set<std::string>   m_activeRequests;
};

struct Query {
    std::set<std::string>                    m_pendingSignals;
    std::function<void()>                    m_successCallback;
    std::function<void(const grpc::Status&)> m_errorCallback;
};

MetadataCache::MetadataCache(const std::shared_ptr<BrokerAsyncGrpcFacade>& asyncBrokerFacade)
    : m_metadataRequester(std::make_unique<MetadataRequester>(
          asyncBrokerFacade,
          [this](const auto& metadata) { onMetadata(std::forward<decltype(metadata)>(metadata)); },
          [this](const auto& path, auto&& status) {
              onError(std::forward<decltype(path)>(path), std::forward<decltype(status)>(status));
          })) {}

std::shared_ptr<Metadata> MetadataCache::getById(int32_t numericId) const {
    std::shared_lock lock(m_mutex);
    if (auto metadata = m_idMap.find(numericId); metadata != m_idMap.end()) {
        return metadata->second;
    }
    return {};
}

std::shared_ptr<Metadata> MetadataCache::getByPath(const std::string& path) const {
    std::shared_lock lock(m_mutex);
    if (auto metadata = m_pathMap.find(path); metadata != m_pathMap.end()) {
        return metadata->second;
    }
    return {};
}

bool MetadataCache::isPresent(const std::string& path) const {
    return (m_pathMap.find(path) != m_pathMap.cend());
}

std::deque<std::string>
MetadataCache::determineMissingSignals(const std::vector<std::string>& signalPaths) const {
    std::deque<std::string> missingSignals;
    {
        std::shared_lock lock(m_mutex);
        for (const auto& path : signalPaths) {
            if (!isPresent(path)) {
                missingSignals.push_back(path);
            }
        }
    }
    return missingSignals;
}

void MetadataCache::addQuery(Query&& query) {
    std::unique_lock lock(m_mutex);
    m_pendingQueries.push_back(std::move(query));
}

void MetadataCache::query(const std::vector<std::string>&          signalPaths,
                          std::function<void()>                    onSuccess,
                          std::function<void(const grpc::Status&)> onError) {
    auto missingSignals = determineMissingSignals(signalPaths);
    if (missingSignals.empty()) {
        onSuccess();
    } else {
        addQuery(Query{std::set(missingSignals.cbegin(), missingSignals.cend()),
                       std::move(onSuccess), std::move(onError)});
        m_metadataRequester->addSignalsToRequest(std::move(missingSignals));
    }
}

void MetadataCache::invalidate(grpc::StatusCode statusCode) {
    std::deque<Query> openQueries;
    {
        std::unique_lock lock(m_mutex);
        m_metadataRequester->reset();
        m_idMap.clear();
        m_pathMap.clear();
        m_pendingQueries.swap(openQueries);
    }
    notifyQueryInitiators(std::move(openQueries), grpc::Status(statusCode, "Cache invalidation"));
}

std::deque<Query> MetadataCache::withdrawFulfilledQueries(const std::string& path) {
    std::deque<Query> fulfilledQueries;
    auto              queryIter = m_pendingQueries.begin();
    while (queryIter != m_pendingQueries.end()) {
        queryIter->m_pendingSignals.erase(path);
        if (queryIter->m_pendingSignals.empty()) {
            fulfilledQueries.push_back(std::move(*queryIter));
            queryIter = m_pendingQueries.erase(queryIter);
        } else {
            ++queryIter;
        }
    }
    return fulfilledQueries;
}

void MetadataCache::onMetadata(const std::shared_ptr<Metadata>& metadata) {
    assert(metadata);
    std::deque<Query> fulfilledQueries;
    {
        std::unique_lock lock(m_mutex);
        m_pathMap[metadata->m_path] = metadata;
        if (metadata->m_isKnown) {
            m_idMap[metadata->m_id] = metadata;
        }
        fulfilledQueries = withdrawFulfilledQueries(metadata->m_path);
    }
    notifyQueryInitiators(std::move(fulfilledQueries));
}

std::deque<Query> MetadataCache::withdrawAffectedQueries(const std::string& path) {
    std::deque<Query> affectedQueries;
    auto              queryIter = m_pendingQueries.begin();
    while (queryIter != m_pendingQueries.end()) {
        if (queryIter->m_pendingSignals.count(path) > 0) {
            affectedQueries.push_back(std::move(*queryIter));
            queryIter = m_pendingQueries.erase(queryIter);
        } else {
            ++queryIter;
        }
    }
    return affectedQueries;
}

void MetadataCache::onError(const std::string& path, grpc::Status&& status) {
    std::deque<Query> affectedQueries;
    {
        std::unique_lock lock(m_mutex);
        affectedQueries = withdrawAffectedQueries(path);
    }
    notifyQueryInitiators(std::move(affectedQueries), std::move(status));
}

void MetadataCache::notifyQueryInitiators(std::deque<Query>&& fulfilledQueries) const {
    for (const auto& query : fulfilledQueries) {
        try {
            query.m_successCallback();
        } catch (...) {
            logger().error("Unknown exception catched while notifying query initiators");
        }
    }
}

void MetadataCache::notifyQueryInitiators(std::deque<Query>&& affectedQueries,
                                          grpc::Status&&      status) const {
    for (const auto& query : affectedQueries) {
        try {
            query.m_errorCallback(std::move(status));
        } catch (...) {
            logger().error("Unknown exception catched while notifying query initiators");
        }
    }
}

} // namespace velocitas::kuksa_val_v2
