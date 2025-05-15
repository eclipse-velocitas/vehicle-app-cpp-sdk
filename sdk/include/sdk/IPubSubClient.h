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

#ifndef VEHICLE_APP_SDK_IPUBSUBCLIENT_H
#define VEHICLE_APP_SDK_IPUBSUBCLIENT_H

#include "sdk/AsyncResult.h"

#include <memory>
#include <string>

namespace velocitas {

/**
 * @brief Interface for implementing PubSub clients.
 *
 */
class IPubSubClient {
public:
    /**
     * @brief Create an instance of a pub/sub client according to the defined middleware
     * configuration
     *
     * @param clientId used to identify the client at the pub/sub server
     * @return std::shared_ptr<IPubSubClient> reference to the created pub/sub client
     */
    static std::shared_ptr<IPubSubClient> createInstance(const std::string& clientId);

    /**
     * @brief Create a new instance of an MQTT client connecting to a broker at the specified
     * address
     *
     * @param brokerUri address of the MQTT broker to connect to
     * @param clientId used to identify the client at the MQTT broker
     * @return std::shared_ptr<IPubSubClient> reference to the created MQTT client
     */
    static std::shared_ptr<IPubSubClient> createInstance(const std::string& brokerUri,
                                                         const std::string& clientId);

    /**
     * @brief Create a new instance of an MQTT client connecting to a broker at the specified
     * address using the provided credentials
     *
     * @param brokerUri address of the MQTT broker to connect to
     * @param clientId used to identify the client at the MQTT broker
     * @param username username to get access to the MQTT broker
     * @param password password to get access to the MQTT broker
     * @return std::shared_ptr<IPubSubClient> reference to the created MQTT client
     */
    static std::shared_ptr<IPubSubClient> createInstance(const std::string& brokerUri,
                                                         const std::string& clientId,
                                                         const std::string& username,
                                                         const std::string& password);

    /**
     * @brief Create a new instance of an MQTT client connecting to a broker at the specified
     * address using the provided credentials
     *
     * @param brokerUri address of the MQTT broker to connect to
     * @param clientId used to identify the client at the MQTT broker
     * @param token used to get access to the MQTT broker
     * @return std::shared_ptr<IPubSubClient> reference to the created MQTT client
     */
    static std::shared_ptr<IPubSubClient> createInstance(const std::string& brokerUri,
                                                         const std::string& clientId,
                                                         const std::string& token);

    /**
     * @brief Create a new instance of an MQTT client connecting to a broker at the specified
     * address using the provided credentials
     *
     * @param brokerUri address of the MQTT broker to connect to
     * @param clientId used to identify the client at the MQTT broker
     * @param trustStorePath The file containing the public digital certificates trusted by the
     * client.
     * @param keyStorePath The file containing the public certificate chain of the client.
     * @param privateKeyPath The file containing the client's private key.
     * @return std::shared_ptr<IPubSubClient> reference to the created MQTT client
     */
    static std::shared_ptr<IPubSubClient> createInstance(const std::string& brokerUri,
                                                         const std::string& clientId,
                                                         const std::string& trustStorePath,
                                                         const std::string& keyStorePath,
                                                         const std::string& privateKeyPath);

    virtual ~IPubSubClient() = default;

    /**
     * @brief Connect the client to the broker.
     *
     */
    virtual void connect() = 0;

    /**
     * @brief Disconnect the client from the broker.
     *
     */
    virtual void disconnect() = 0;

    /**
     * @brief Return the connection state.
     *
     * @return true
     * @return false
     */
    [[nodiscard]] virtual bool isConnected() const = 0;

    /**
     * @brief Publish a message on a topic.
     *
     * @param topic   The topic to which to publish.
     * @param data    The message data.
     */
    virtual void publishOnTopic(const std::string& topic, const std::string& data) = 0;

    /**
     * @brief Subscribe to a topic.
     *
     * @param topic   The topic to subscribe to.
     * @return AsyncSubscriptionPtr_t<std::string>  The subscription to the topic.
     */
    virtual AsyncSubscriptionPtr_t<std::string> subscribeTopic(const std::string& topic) = 0;

    IPubSubClient(const IPubSubClient&)            = delete;
    IPubSubClient(IPubSubClient&&)                 = delete;
    IPubSubClient& operator=(const IPubSubClient&) = delete;
    IPubSubClient& operator=(IPubSubClient&&)      = delete;

protected:
    IPubSubClient() = default;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_IPUBSUBCLIENT_H
