/**
 * Copyright (c) 2023-2025 Contributors to the Eclipse Foundation
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

#include "NativeMiddleware.h"

#include "sdk/Utils.h"

#include <fmt/core.h>

#include <memory>
#include <string>

namespace velocitas {

std::unique_ptr<Middleware> Middleware::instantiate() {
    const std::string middlewareType = StringUtils::toLower(getEnvVar(TYPE_DEFINING_ENV_VAR_NAME));
    if (middlewareType.empty()) {
        return std::make_unique<NativeMiddleware>();
    } else if (middlewareType == NativeMiddleware::TYPE_ID) {
        return std::make_unique<NativeMiddleware>();
    } else {
        throw std::runtime_error(fmt::format("Unknown middleware type '{}'", middlewareType));
    }
}

} // namespace velocitas
