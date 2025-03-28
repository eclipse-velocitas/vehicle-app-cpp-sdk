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

#include <grpcpp/support/status_code_enum.h>

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace grpc {
class Status;
}

namespace velocitas::kuksa_val_v2 {

/** Numeric id type as used by the KUKSA Databroker for referencing signals */
using numeric_id_t = int32_t;

struct Metadata {
    std::string  m_signalPath;
    numeric_id_t m_id{0};
    bool         m_isKnown{false};
};

using MetadataPtr_t  = std::shared_ptr<Metadata>;
using MetadataList_t = std::vector<MetadataPtr_t>;

using SignalPathList_t = std::vector<std::string>;

class BrokerAsyncGrpcFacade;

/**
 * Provides access to (VSS) metadata hosted by the KUKSA Databroker.
 *
 * Maintaines a cache of previously returned metadata from the databroker.
 */
class MetadataAgent {
public:
    static std::shared_ptr<MetadataAgent> create(const std::shared_ptr<BrokerAsyncGrpcFacade>&);
    virtual ~MetadataAgent() = default;

    /**
     * @brief Asynchronously provides metadata for the passed set of signals.
     *
     * @param signalPaths List of signal paths to provide metadata for
     * @param onSuccess This function is called once metadata for all requested signals is present.
     * Be aware, that this can either happen immediately - i.e. before returning from this call - if
     * all required signals are already present in the internal cache!
     * @param onError This function is called if metadata not for all required signals can be
     * gained, i.e. an unexpected error is occuring or the cache is invalidated while waiting for
     * the requested metadata.
     */
    virtual void query(const SignalPathList_t&                    signalPaths,
                       std::function<void(MetadataList_t&&)>&&    onSuccess,
                       std::function<void(const grpc::Status&)>&& onError) = 0;

    /**
     * @brief Invalidates the cached metadata. This needs to be called by clients when they get
     * aware that the databroker was restarted.
     *
     * @param statusCode
     */
    virtual void invalidate(grpc::StatusCode statusCode = grpc::StatusCode::UNAVAILABLE) = 0;

    /**
     * @brief Get metadata of a signal reference by its numeric id.
     *
     * @param mumericId Session-related (i.e. temporary) numeric id used by the databroker to
     * reference a signal (besides its non-temporary path/name)
     * @return MetadataPtr_t Points to the metadata referenced by the numeric id. A
     * nullptr is returned if the passed id is unknown.
     */
    [[nodiscard]] virtual MetadataPtr_t getByNumericId(numeric_id_t mumericId) const = 0;
};

} // namespace velocitas::kuksa_val_v2

#endif // VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_METADATA_H
