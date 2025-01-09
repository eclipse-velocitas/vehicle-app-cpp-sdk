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

#ifndef VEHICLE_APP_SDK_MIDDLEWARE_MIDDLEWARE_H
#define VEHICLE_APP_SDK_MIDDLEWARE_MIDDLEWARE_H

#include <memory>
#include <string>
#include <unordered_map>

namespace velocitas {

class IPubSubClient;

/**
 * @brief Abstract base class for concrete middleware implementations
 */
class Middleware {
public:
    /**
     * @brief Defines the name of the environment variable used to determine the middleware type to
     * be used.
     */
    static constexpr char const* TYPE_DEFINING_ENV_VAR_NAME = "SDV_MIDDLEWARE_TYPE";

    /**
     * @brief Returns a reference to a singelton instance of a concrete middleware class
     *
     * @return Middleware&
     */
    static Middleware& getInstance() {
        static std::unique_ptr<Middleware> singleton = instantiate();
        return *singleton;
    }

    /**
     * @brief Get the type identifier of the concrete middleware implementation
     *
     * @return std::string type of the middleware
     */
    [[nodiscard]] std::string getTypeId() const { return m_typeId; }

    /**
     * @brief Triggers the start of the middleware
     */
    virtual void start() {}
    /**
     * @brief Waits (blocks current thread) until the middleware is started and ready to use
     */
    virtual void waitUntilReady() {}
    /**
     * @brief Stops the middleware
     */
    virtual void stop() {}

    /**
     * @brief Get the location description (e.g. uri) of the specified service name
     *
     * @param serviceName Name of the service to get the loaction description for
     * @return std::string representing the location description
     * @throws std::runtime_error if the service location cannot be determined
     */
    [[nodiscard]] virtual std::string getServiceLocation(const std::string& serviceName) const = 0;

    /**
     * @brief Generic type for middleware specific metadata     *
     */
    using Metadata = std::unordered_map<std::string, std::string>;

    /**
     * @brief Get the middleware specific metadata needed to communicate with the specified service.
     *
     * @param serviceName Name of the service to communicate with
     * @return Metadata
     */
    [[nodiscard]] virtual Metadata getMetadata(const std::string& serviceName) const {
        std::ignore = serviceName;
        return Metadata{};
    }

    /**
     * @brief Create a Pub Sub Client object
     *
     * @param clientId
     * @return std::shared_ptr<IPubSubClient>
     */
    [[nodiscard]] virtual std::shared_ptr<IPubSubClient>
    createPubSubClient(const std::string& clientId) const = 0;

protected:
    /**
     * @brief Constructor to be called by subclasses
     *
     * @param typeId unique identifier for the type of the concrete middleware
     * implementation
     */
    explicit Middleware(std::string&& typeId)
        : m_typeId(std::move(typeId)) {}

private:
    static std::unique_ptr<Middleware> instantiate();

    const std::string m_typeId;

    Middleware(const Middleware&)            = delete;
    Middleware(Middleware&&)                 = delete;
    Middleware& operator=(const Middleware&) = delete;
    Middleware& operator=(Middleware&&)      = delete;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_MIDDLEWARE_MIDDLEWARE_H
