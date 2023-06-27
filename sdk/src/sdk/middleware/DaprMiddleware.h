/**
 * Copyright (c) 2023 Robert Bosch GmbH
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

#ifndef VEHICLE_APP_SDK_MIDDLEWARE_DAPRMIDDLEWARE_H
#define VEHICLE_APP_SDK_MIDDLEWARE_DAPRMIDDLEWARE_H

#include "sdk/middleware/Middleware.h"

namespace velocitas {

class DaprMiddleware : public Middleware {
public:
    static constexpr char const* TYPE_ID = "dapr";

    DaprMiddleware()
        : Middleware(TYPE_ID) {}

    void waitUntilReady() override;

    std::string getServiceLocation(const std::string& serviceName) const override;
    Metadata    getMetadata(const std::string& serviceName) const override;
    std::shared_ptr<IPubSubClient> createPubSubClient(const std::string& clientId) const override;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_MIDDLEWARE_DAPRMIDDLEWARE_H
