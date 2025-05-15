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

#ifndef VEHICLE_APP_SDK_VEHICLEMODELCONTEXT_H
#define VEHICLE_APP_SDK_VEHICLEMODELCONTEXT_H

#include "sdk/AsyncResult.h"
#include "sdk/DataPoint.h"

#include <memory>

namespace velocitas {

class IVehicleDataBrokerClient;

/**
 * @brief Abstraction layer for vehicle models which provides access
 * to the actual implementation that drives the fluent queries (get/set/subscribe)
 *
 */
class VehicleModelContext final {
public:
    /**
     * @brief Provide the instance of the context.
     *
     * @return VehicleModelContext&  The context instance.
     */
    static VehicleModelContext& getInstance() {
        static VehicleModelContext context{};
        return context;
    }

    /**
     * @brief Set the VehicleDataBrokerClient implementation.
     *
     * @param vdbc Pointer to the VehicleDataBrokerClient
     */
    void setVdbc(std::shared_ptr<IVehicleDataBrokerClient> vdbc) { m_vdbc = vdbc; }

    /**
     * @brief Get the VehicleDataBrokerClient implementation.
     *
     * @return std::shared_ptr<IVehicleDataBrokerClient>  Pointer to the VehicleDataBrokerClient
     * implementation.
     */
    std::shared_ptr<IVehicleDataBrokerClient> getVdbc() { return m_vdbc; }

private:
    VehicleModelContext() = default;

    std::shared_ptr<IVehicleDataBrokerClient> m_vdbc;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_VEHICLEMODELCONTEXT_H
