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

#include "sdk/Job.h"
#include "sdk/Logger.h"
#include "sdk/ThreadPool.h"
#include "sdk/vdb/grpc/kuksa_val_v2/BrokerAsyncGrpcFacade.h"

#include <algorithm>
#include <deque>
#include <limits>
#include <set>
#include <shared_mutex>
#include <stdexcept>
#include <utility>

namespace velocitas::kuksa_val_v2 {

namespace {

const unsigned int MAX_PARALLEL_REQUESTS{5};

class Request {
public:
    static std::shared_ptr<Request>
    create(const std::string&                                                           signalPath,
           std::function<void(const std::shared_ptr<Request>&, const MetadataPtr_t&)>&& onMetadata,
           std::function<void(const std::shared_ptr<Request>&, const grpc::Status&)>&&  onError) {
        auto request = std::shared_ptr<Request>(
            new Request(signalPath, std::move(onMetadata), std::move(onError)));
        request->setThisPtr(request);
        return request;
    }

    void initiate(const std::shared_ptr<BrokerAsyncGrpcFacade>& brokerFacade) {
        ThreadPool::getInstance()->enqueue(Job::create([thisPtr = getThisPtr(), brokerFacade]() {
            // !! Capturing a shared_ptr to this Request object (i.e. thisPtr) within this lambda
            // guarantees that this object is not destructed before the lambda is left, means
            // destruction happens outside any function of this class.
            if (thisPtr->m_isCancelled) {
                thisPtr->onError(grpc::Status(grpc::StatusCode::CANCELLED, ""));
            } else {
                kuksa::val::v2::ListMetadataRequest request;
                request.set_root(thisPtr->m_signalPath);
                brokerFacade->ListMetadata(
                    std::move(request),
                    [thisPtr](const auto& response) {
                        thisPtr->onResponse(std::forward<decltype(response)>(response));
                    },
                    [thisPtr](const auto& status) {
                        thisPtr->onError(std::forward<decltype(status)>(status));
                    });
            }
        }));
    }

    void                             cancel() { m_isCancelled = true; }
    [[nodiscard]] bool               isCancelled() const { return m_isCancelled; }
    [[nodiscard]] const std::string& getSignalPath() const { return m_signalPath; }

private:
    Request(std::string                                                                  signalPath,
            std::function<void(const std::shared_ptr<Request>&, const MetadataPtr_t&)>&& onMetadata,
            std::function<void(const std::shared_ptr<Request>&, const grpc::Status&)>&&  onError)
        : m_signalPath(std::move(signalPath))
        , m_metadataCallback(std::move(onMetadata))
        , m_errorCallback(std::move(onError)) {}

    void setThisPtr(const std::shared_ptr<Request>& thisSharedPtr) { m_this = thisSharedPtr; }
    [[nodiscard]] std::shared_ptr<Request> getThisPtr() const { return m_this.lock(); }

    void onResponse(const kuksa::val::v2::ListMetadataResponse& response) {
        if (!m_isCancelled) {
            if (response.metadata_size() == 1) {
                m_metadataCallback(getThisPtr(),
                                   std::make_shared<Metadata>(
                                       Metadata{m_signalPath, response.metadata(0).id(), true}));
            } else {
                if (response.metadata_size() == 0) {
                    logger().warn("Databroker returned empty metadata list for {} -> "
                                  "assuming signal as 'unknown'",
                                  m_signalPath);
                } else {
                    logger().warn("Databroker returned multiple metadata entries for {} -> "
                                  "assuming signal as 'unknown'",
                                  m_signalPath);
                }
                m_metadataCallback(getThisPtr(),
                                   std::make_shared<Metadata>(Metadata{m_signalPath, 0, false}));
            }
        } else {
            m_errorCallback(getThisPtr(), grpc::Status(grpc::StatusCode::CANCELLED, ""));
        }
    };

    void onError(const grpc::Status& status) {
        if (!m_isCancelled) {
            if (status.error_code() == grpc::StatusCode::NOT_FOUND ||
                status.error_code() == grpc::StatusCode::PERMISSION_DENIED) {
                m_metadataCallback(getThisPtr(),
                                   std::make_shared<Metadata>(Metadata{m_signalPath, 0, false}));
            } else {
                m_errorCallback(getThisPtr(), status);
            }
        } else {
            m_errorCallback(getThisPtr(), grpc::Status(grpc::StatusCode::CANCELLED, ""));
        }
    }

    const std::string                                                          m_signalPath;
    std::weak_ptr<Request>                                                     m_this;
    std::atomic<bool>                                                          m_isCancelled{false};
    std::function<void(const std::shared_ptr<Request>&, const MetadataPtr_t&)> m_metadataCallback;
    std::function<void(const std::shared_ptr<Request>&, const grpc::Status&)>  m_errorCallback;
};

class MetadataCache {
public:
    void add(const MetadataPtr_t& metadata) {
        assert(metadata);
        m_pathMap[metadata->m_signalPath] = metadata;
        if (metadata->m_isKnown) {
            m_idMap[metadata->m_id] = metadata;
        }
    }

    void clear() {
        m_pathMap.clear();
        m_idMap.clear();
    }

    [[nodiscard]] bool isPresent(const std::string& signalPath) const {
        return (m_pathMap.find(signalPath) != m_pathMap.cend());
    }

    [[nodiscard]] MetadataPtr_t getByPath(const std::string& signalPath) const {
        if (auto metadata = m_pathMap.find(signalPath); metadata != m_pathMap.end()) {
            return metadata->second;
        }
        return {};
    }

    [[nodiscard]] MetadataPtr_t getById(numeric_id_t numericId) const {
        if (auto metadata = m_idMap.find(numericId); metadata != m_idMap.end()) {
            return metadata->second;
        }
        return {};
    }

private:
    std::map<std::string, MetadataPtr_t>  m_pathMap;
    std::map<numeric_id_t, MetadataPtr_t> m_idMap;
};

class Query {
public:
    Query(const SignalPathList_t&                    signalPaths,
          std::function<void(MetadataList_t&&)>&&    successCallback,
          std::function<void(const grpc::Status&)>&& errorCallback)
        : m_missingSignals(signalPaths.cbegin(), signalPaths.cend())
        , m_successCallback(std::move(successCallback))
        , m_errorCallback(std::move(errorCallback)) {
        m_cachedMetadata.reserve(signalPaths.size());
    }

    void addMetadata(const MetadataPtr_t& metadata) {
        auto numErased = m_missingSignals.erase(metadata->m_signalPath);
        if (numErased > 0) {
            m_cachedMetadata.push_back(metadata);
        }
    }
    [[nodiscard]] bool isFulfilled() const { return m_missingSignals.empty(); }
    [[nodiscard]] const std::set<std::string>& getMissingSignals() const {
        return m_missingSignals;
    }

    void notifyInitiator() {
        assert(m_missingSignals.empty());
        try {
            m_successCallback(std::move(m_cachedMetadata));
        } catch (const std::exception& e) {
            logger().error("Exception catched while notifying metadata query success: ", e.what());
        } catch (...) {
            logger().error("Unknown exception catched while notifying metadata query success");
        }
    }

    void notifyInitiator(const grpc::Status& status) {
        try {
            m_errorCallback(status);
        } catch (const std::exception& e) {
            logger().error("Exception catched while notifying metadata query error: ", e.what());
        } catch (...) {
            logger().error("Unknown exception catched while notifying metadata query error");
        }
    }

private:
    MetadataList_t                           m_cachedMetadata;
    std::set<std::string>                    m_missingSignals;
    std::function<void(MetadataList_t&&)>    m_successCallback;
    std::function<void(const grpc::Status&)> m_errorCallback;
};

void notifyQueryInitiators(std::deque<Query>&& queries) {
    for (auto& query : queries) {
        query.notifyInitiator();
    }
}

void notifyQueryInitiators(std::deque<Query>&& queries, const grpc::Status& status) {
    for (auto& query : queries) {
        query.notifyInitiator(status);
    }
}

} // namespace

class MetadataAgentImpl : public MetadataAgent {
public:
    explicit MetadataAgentImpl(std::shared_ptr<BrokerAsyncGrpcFacade> asyncBrokerFacade)
        : m_asyncBrokerFacade(std::move(asyncBrokerFacade)) {}

    void query(const SignalPathList_t&                    signalPaths,
               std::function<void(MetadataList_t&&)>&&    onSuccess,
               std::function<void(const grpc::Status&)>&& onError) override;
    void invalidate(grpc::StatusCode statusCode) override;

    [[nodiscard]] MetadataPtr_t getByNumericId(numeric_id_t numericId) const override {
        std::shared_lock lock(m_mutex);
        return m_cache.getById(numericId);
    }

private:
    void              addCachedMetadata(Query& query, const SignalPathList_t& signalPaths);
    void              addQuery(Query&& query);
    void              addSetOfSignalToRequestQueue(const std::set<std::string>& signals);
    void              addSignalToRequestQueue(const std::string& signalPath);
    bool              isSignalPartOfActiveRequests(const std::string& signalPath) const;
    void              triggerMetadataRequests();
    void              updateActiveRequests(const std::shared_ptr<Request>& request);
    std::deque<Query> updateQueriesAndExtractFulfilled(const MetadataPtr_t& metadata);
    std::deque<Query> extractAffectedQueries(const std::string& signalPath);
    void              cancelActiveRequests();

    std::shared_ptr<BrokerAsyncGrpcFacade> m_asyncBrokerFacade;

    mutable std::shared_mutex          m_mutex;
    MetadataCache                      m_cache;
    std::deque<Query>                  m_pendingQueries;
    std::deque<std::string>            m_pendingSignals;
    std::set<std::shared_ptr<Request>> m_activeRequests;
};

std::shared_ptr<MetadataAgent>
MetadataAgent::create(const std::shared_ptr<BrokerAsyncGrpcFacade>& brokerFacade) {
    return std::make_shared<MetadataAgentImpl>(brokerFacade);
}

void MetadataAgentImpl::cancelActiveRequests() {
    for (const auto& request : m_activeRequests) {
        request->cancel();
    }
}

void MetadataAgentImpl::invalidate(grpc::StatusCode statusCode) {
    logger().info("Invalidating signal metadata cache");
    std::deque<Query> openQueries;
    {
        std::unique_lock lock(m_mutex);
        m_cache.clear();
        m_pendingQueries.swap(openQueries);
        m_pendingSignals.clear();
        cancelActiveRequests();
    }
    notifyQueryInitiators(std::move(openQueries), grpc::Status(statusCode, "Cache invalidation"));
}

void MetadataAgentImpl::addCachedMetadata(Query& query, const SignalPathList_t& signalPaths) {
    for (const auto& path : signalPaths) {
        if (auto metadata = m_cache.getByPath(path)) {
            query.addMetadata(metadata);
        }
    }
}

void MetadataAgentImpl::query(const SignalPathList_t&                    signalPaths,
                              std::function<void(MetadataList_t&&)>&&    onSuccess,
                              std::function<void(const grpc::Status&)>&& onError) {
    Query query(signalPaths, std::move(onSuccess), std::move(onError));
    {
        std::unique_lock lock(m_mutex);
        addCachedMetadata(query, signalPaths);
        if (!query.isFulfilled()) {
            addQuery(std::move(query));
            return;
        }
    }
    query.notifyInitiator();
}

void MetadataAgentImpl::addQuery(Query&& query) {
    addSetOfSignalToRequestQueue(query.getMissingSignals());
    m_pendingQueries.push_back(std::move(query));
    triggerMetadataRequests();
}

bool MetadataAgentImpl::isSignalPartOfActiveRequests(const std::string& signalPath) const {
    return std::any_of(
        m_activeRequests.cbegin(), m_activeRequests.cend(),
        [signalPath](const auto& request) { return signalPath == request->getSignalPath(); });
}

void MetadataAgentImpl::addSignalToRequestQueue(const std::string& signalPath) {
    if (!isSignalPartOfActiveRequests(signalPath) &&
        (std::find(m_pendingSignals.cbegin(), m_pendingSignals.cend(), signalPath) ==
         m_pendingSignals.end())) {
        m_pendingSignals.push_back(signalPath);
    }
}

void MetadataAgentImpl::addSetOfSignalToRequestQueue(const std::set<std::string>& signals) {
    for (const auto& signalPath : signals) {
        addSignalToRequestQueue(signalPath);
    }
}

void MetadataAgentImpl::triggerMetadataRequests() {
    while (!m_pendingSignals.empty() && (m_activeRequests.size() < MAX_PARALLEL_REQUESTS)) {
        auto request = Request::create(
            m_pendingSignals.front(),
            [this](const auto& request, const auto& metadata) {
                assert(request && metadata);
                std::deque<Query> fulfilledQueries;
                {
                    std::unique_lock lock(m_mutex);
                    updateActiveRequests(request);
                    if (!request->isCancelled()) {
                        m_cache.add(metadata);
                        fulfilledQueries = updateQueriesAndExtractFulfilled(metadata);
                    }
                }
                notifyQueryInitiators(std::move(fulfilledQueries));
            },
            [this](const auto& request, const auto& status) {
                assert(request);
                std::deque<Query> affectedQueries;
                {
                    std::unique_lock lock(m_mutex);
                    updateActiveRequests(request);
                    if (!request->isCancelled()) {
                        affectedQueries = extractAffectedQueries(request->getSignalPath());
                    }
                }
                notifyQueryInitiators(std::move(affectedQueries), status);
            });
        m_pendingSignals.pop_front();
        m_activeRequests.insert(request);
        request->initiate(m_asyncBrokerFacade);
    }
}

void MetadataAgentImpl::updateActiveRequests(const std::shared_ptr<Request>& request) {
    size_t numErasedRequests = m_activeRequests.erase(request);
    assert(numErasedRequests == 1);
    triggerMetadataRequests();
}

std::deque<Query>
MetadataAgentImpl::updateQueriesAndExtractFulfilled(const MetadataPtr_t& metadata) {
    std::deque<Query> fulfilledQueries;
    auto              queryIter = m_pendingQueries.begin();
    while (queryIter != m_pendingQueries.end()) {
        queryIter->addMetadata(metadata);
        if (queryIter->isFulfilled()) {
            fulfilledQueries.push_back(std::move(*queryIter));
            queryIter = m_pendingQueries.erase(queryIter);
        } else {
            ++queryIter;
        }
    }
    return fulfilledQueries;
}

std::deque<Query> MetadataAgentImpl::extractAffectedQueries(const std::string& signalPath) {
    std::deque<Query> affectedQueries;
    auto              queryIter = m_pendingQueries.begin();
    while (queryIter != m_pendingQueries.end()) {
        if (queryIter->getMissingSignals().count(signalPath) > 0) {
            affectedQueries.push_back(std::move(*queryIter));
            queryIter = m_pendingQueries.erase(queryIter);
        } else {
            ++queryIter;
        }
    }
    return affectedQueries;
}

} // namespace velocitas::kuksa_val_v2
