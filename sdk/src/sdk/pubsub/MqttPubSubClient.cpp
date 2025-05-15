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

#include "sdk/IPubSubClient.h"
#include "sdk/Logger.h"
#include "sdk/Status.h"
#include "sdk/ThreadPool.h"

#include "sdk/middleware/Middleware.h"

#include <mqtt/async_client.h>
#include <mqtt/connect_options.h>
#include <unordered_map>

namespace velocitas {

class MqttPubSubClient : public IPubSubClient, private mqtt::callback {
public:
    MqttPubSubClient()                                   = delete;
    MqttPubSubClient(const MqttPubSubClient&)            = delete;
    MqttPubSubClient(MqttPubSubClient&&)                 = delete;
    MqttPubSubClient& operator=(const MqttPubSubClient&) = delete;
    MqttPubSubClient& operator=(MqttPubSubClient&&)      = delete;

    MqttPubSubClient(const std::string& brokerUri, const std::string& clientId)
        : m_client{brokerUri, clientId}
        , m_connectOptions{} {
        m_client.set_callback(*this);
    }

    MqttPubSubClient(const std::string& brokerUri, const std::string& clientId,
                     const std::string& username, const std::string& password)
        : m_client{brokerUri, clientId}
        , m_connectOptions{username, password} {
        m_client.set_callback(*this);
    }

    MqttPubSubClient(const std::string& brokerUri, const std::string& clientId,
                     const std::string& token)
        : m_client{brokerUri, clientId} {
        m_client.set_callback(*this);
        m_connectOptions = mqtt::connect_options_builder().user_name(token).finalize();
    }

    MqttPubSubClient(const std::string& brokerUri, const std::string& clientId,
                     const std::string& trustStorePath, const std::string& keyStorePath,
                     const std::string& privateKeyPath)
        : m_client{brokerUri, clientId} {
        m_client.set_callback(*this);
        auto sslopts = mqtt::ssl_options_builder()
                           .trust_store(trustStorePath)
                           .key_store(keyStorePath)
                           .private_key(privateKeyPath)
                           .error_handler([](const std::string& msg) {
                               logger().error("SSL Error: {}", msg);
                           })
                           .finalize();
        m_connectOptions = mqtt::connect_options_builder().ssl(std::move(sslopts)).finalize();
    }

    ~MqttPubSubClient() override = default;

    void connect() override {
        logger().info("Connecting to MQTT broker at '{}' with client-id '{}'",
                      m_client.get_server_uri(), m_client.get_client_id());

        /* Backward "compatibility warning */
        if (getenv("MQTT_BROKER_URI") != nullptr) {
            logger().warn("... ignoring deprecated environment variable MQTT_BROKER_URI -> "
                          "consider to remove it");
        }

        m_client.connect(m_connectOptions)->wait();
    }
    void               disconnect() override { m_client.disconnect()->wait(); }
    [[nodiscard]] bool isConnected() const override { return m_client.is_connected(); }

    void publishOnTopic(const std::string& topic, const std::string& data) override {
        logger().debug(R"(Publish on topic "{}": "{}")", topic, data);
        m_client.publish(topic, data)->wait();
    }

    AsyncSubscriptionPtr_t<std::string> subscribeTopic(const std::string& topic) override {
        logger().debug("Subscribing to {}", topic);
        auto subscription = std::make_shared<AsyncSubscription<std::string>>();
        m_subscriberMap.insert(std::make_pair(topic, subscription));
        m_client.subscribe(topic, 0)->wait();
        return subscription;
    }

private:
    void message_arrived(mqtt::const_message_ptr msg) override {
        const std::string& topic   = msg->get_topic();
        const std::string& payload = msg->get_payload_str();
        logger().debug(R"(MQTT: Update on topic "{}": "{}")", topic, payload);

        // Todo: Replace by solution capable handling wildcards
        auto range = m_subscriberMap.equal_range(topic);
        for (auto it = range.first; it != range.second; ++it) {
            auto subscription = it->second;
            ThreadPool::getInstance()->enqueue(Job::create([subscription, payload]() {
                try {
                    subscription->insertNewItem(std::string(payload));
                } catch (std::exception& e) {
                    subscription->insertError(Status(
                        fmt::format("MQTT: Callback threw an exception on update: {}", e.what())));
                }
            }));
        }
    }

    using TopicMap_t =
        std::unordered_multimap<std::string, std::shared_ptr<AsyncSubscription<std::string>>>;

    mqtt::async_client    m_client;
    mqtt::connect_options m_connectOptions;
    TopicMap_t            m_subscriberMap;
};

std::shared_ptr<IPubSubClient> IPubSubClient::createInstance(const std::string& clientId) {
    return Middleware::getInstance().createPubSubClient(clientId);
}

std::shared_ptr<IPubSubClient> IPubSubClient::createInstance(const std::string& brokerUri,
                                                             const std::string& clientId) {
    return std::make_shared<MqttPubSubClient>(brokerUri, clientId);
}

std::shared_ptr<IPubSubClient> IPubSubClient::createInstance(const std::string& brokerUri,
                                                             const std::string& clientId,
                                                             const std::string& username,
                                                             const std::string& password) {
    return std::make_shared<MqttPubSubClient>(brokerUri, clientId, username, password);
}
std::shared_ptr<IPubSubClient> IPubSubClient::createInstance(const std::string& brokerUri,
                                                             const std::string& clientId,
                                                             const std::string& token) {
    return std::make_shared<MqttPubSubClient>(brokerUri, clientId, token);
}

std::shared_ptr<IPubSubClient> IPubSubClient::createInstance(const std::string& brokerUri,
                                                             const std::string& clientId,
                                                             const std::string& trustStorePath,
                                                             const std::string& keyStorePath,
                                                             const std::string& privateKeyPath) {
    return std::make_shared<MqttPubSubClient>(brokerUri, clientId, trustStorePath, keyStorePath,
                                              privateKeyPath);
}

} // namespace velocitas
