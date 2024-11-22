/**
 * Copyright (c) 2024 Contributors to the Eclipse Foundation
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

#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include "sdk/Logger.h"
#include "sdk/Utils.h"
#include "sdk/vdb/grpc/kuksa_val_v2/BrokerClient.h"
#include "sdk/vdb/grpc/sdv_databroker_v1/BrokerClient.h"

#include <memory>
#include <stdexcept>
#include <string>

namespace velocitas {

static const std::string API_DEFINING_ENV_VAR = "KUKSA_DATABROKER_API"; // NOLINT(runtime/string)
static const std::string SDV_V1_API           = "sdv.databroker.v1";    // NOLINT(runtime/string)
static const std::string KUKSA_V2_API         = "kuksa.val.v2";         // NOLINT(runtime/string)
static const auto&       DEFAULT_API          = SDV_V1_API;

std::shared_ptr<IVehicleDataBrokerClient>
IVehicleDataBrokerClient::createInstance(const std::string& vdbServiceName) {
    const auto apiVariant = getEnvVar(API_DEFINING_ENV_VAR, DEFAULT_API);

    if (apiVariant == SDV_V1_API) {
        logger().info("Using Kuksa Databroker {} API", apiVariant);
        return std::make_shared<sdv_databroker_v1::BrokerClient>(vdbServiceName);
    }

    if (apiVariant == KUKSA_V2_API) {
        logger().info("Using Kuksa Databroker {} API", apiVariant);
        return std::make_shared<kuksa_val_v2::BrokerClient>(vdbServiceName);
    }

    logger().error("Unsupported Kuksa Databroker {} API", apiVariant);
    throw std::runtime_error("Unsupported API specified");
}

} // namespace velocitas
