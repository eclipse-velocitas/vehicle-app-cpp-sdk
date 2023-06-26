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

#include "sdk/middleware/Middleware.h"

#include "DaprMiddleware.h"
#include "NativeMiddleware.h"

#include "sdk/Utils.h"

#include <fmt/core.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace velocitas {

static constexpr char const* ENV_MIDDLEWARE_TYPE = "SDV_MIDDLEWARE_TYPE";

static const std::unordered_map<std::string, Middleware::Type> MIDDLEWARE_NAME_TYPE_MAP = {
    {"", Middleware::Type::DAPR}, // default
    {"dapr", Middleware::Type::DAPR},
    {"native", Middleware::Type::NATIVE},
};

static Middleware::Type getMiddlewareType(const std::string& middlewareName) {
    auto iter = MIDDLEWARE_NAME_TYPE_MAP.find(StringUtils::toLower(middlewareName));
    if (iter != MIDDLEWARE_NAME_TYPE_MAP.end()) {
        return iter->second;
    }
    return Middleware::Type::UNKNOWN;
}

std::unique_ptr<Middleware> Middleware::instantiate() {
    std::string      middlewareName = getEnvVar(ENV_MIDDLEWARE_TYPE);
    Middleware::Type middlewareType = getMiddlewareType(middlewareName);
    switch (middlewareType) {
    case Type::NATIVE:
        return std::make_unique<NativeMiddleware>();
    case Type::DAPR:
        return std::make_unique<DaprMiddleware>();
    default:
        throw std::runtime_error(fmt::format("Unknown middleware type '{}' -> {}", middlewareName,
                                             static_cast<int>(middlewareType)));
    }
}

} // namespace velocitas
