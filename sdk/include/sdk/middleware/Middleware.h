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

#ifndef VEHICLE_APP_SDK_MIDDLEWARE_MIDDLEWARE_H
#define VEHICLE_APP_SDK_MIDDLEWARE_MIDDLEWARE_H

#include <memory>
#include <string>
#include <unordered_map>

namespace velocitas {

class Middleware {
public:
    enum class Type { UNKNOWN, NATIVE, DAPR };

    static Middleware& getInstance() {
        static std::unique_ptr<Middleware> singleton = instantiate();
        return *singleton;
    }

    virtual Type getType() const = 0;

    virtual void start() {}
    virtual void waitUntilReady() {}
    virtual void stop() {}

    virtual std::string getServiceLocation(const std::string& serviceName) const = 0;
    using Metadata = std::unordered_map<std::string, std::string>;
    virtual Metadata getMetadata(const std::string& serviceName) const {
        std::ignore = serviceName;
        return Metadata{};
    }

protected:
    Middleware() = default;

private:
    static std::unique_ptr<Middleware> instantiate();

    Middleware(const Middleware&)            = delete;
    Middleware(Middleware&&)                 = delete;
    Middleware& operator=(const Middleware&) = delete;
    Middleware& operator=(Middleware&&)      = delete;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_MIDDLEWARE_MIDDLEWARE_H
