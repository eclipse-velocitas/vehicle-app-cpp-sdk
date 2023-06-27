/**
 * Copyright (c) 2022-2023 Robert Bosch GmbH
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
    static std::shared_ptr<IPubSubClient> createInstance(const std::string& clientId);
    static std::shared_ptr<IPubSubClient> createInstance(const std::string& brokerUri,
                                                         const std::string& clientId);

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
