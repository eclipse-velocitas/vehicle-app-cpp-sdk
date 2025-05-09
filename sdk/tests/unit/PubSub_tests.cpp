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

#include "MockIPubSubClient.h"
#include "sdk/DataPoint.h"
#include "sdk/IPubSubClient.h"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
using namespace velocitas;
using ::testing::_;
using ::testing::Return;
using ::testing::Throw;
class PubSubTest : public ::testing::Test {
protected:
    void SetUp() override {
        client = IPubSubClient::createInstance("localhost:1883", "TestClient");
        mockClient = std::make_shared<MockIPubSubClient>();
        client->connect();
    }
    void TearDown() override {
        try {
            if (client && client->isConnected()) {
                client->disconnect();
            }
        } catch (const std::exception& ex) {
            std::cerr << "TearDown exception ignored: " << ex.what() << std::endl;
        }
    }
    void receivedMessage(const std::string& data) {
        messageReceived = true;
        receivedData = data;
    }
    void waitForMessage() {
        while (!messageReceived) {
            std::this_thread::sleep_for(std::chrono::seconds{1});
        }
        messageReceived = false;
    }
    std::shared_ptr<IPubSubClient> client;
    std::shared_ptr<MockIPubSubClient> mockClient;
    std::string receivedData = "";
    bool messageReceived = false;
};
TEST_F(PubSubTest, subscribeTopic_publishOnTopic_sameTopic) {
    auto subAbc = client->subscribeTopic("a/b/c");
    subAbc->onItem([this](auto&& item) {
        receivedMessage(std::forward<decltype(item)>(item));
    });
    std::string message = "testMessage";
    client->publishOnTopic("a/b/c", message);
    waitForMessage();

    EXPECT_EQ(receivedData, message);
}
TEST_F(PubSubTest, unsubscribeTopic_noMessageReceived) {
    const std::string topic = "test/no_message";
    const std::string message = "test message";

    auto subAbc = client->subscribeTopic(topic);
    subAbc->onItem([this](auto&& item) {
        receivedMessage(std::forward<decltype(item)>(item));
    });
    std::this_thread::sleep_for(std::chrono::seconds{1});
    client->unsubscribeTopic(topic);
    client->publishOnTopic(topic, message);
    std::this_thread::sleep_for(std::chrono::seconds{3});

    EXPECT_FALSE(messageReceived);
    EXPECT_NE(receivedData, message);
}
TEST_F(PubSubTest, publishOnTopic_returnsSuccessWhenWithinTimeout) {
    const std::string topic = "test/success";
    const std::string msg = "success message";
    const int timeoutMs = 1000;

    auto status = client->publishOnTopic(topic, msg, timeoutMs);
    EXPECT_EQ(status, PublishStatus::Success);
}
TEST_F(PubSubTest, publishOnTopic_returnsTimeoutWhenPublishTakesTooLong) {
    const std::string topic = "test/timeout";
    const std::string msg = "timeout message";
    const int timeoutMs = 1000;

    EXPECT_CALL(*mockClient, publishOnTopic(topic, msg, timeoutMs))
        .WillOnce(Return(PublishStatus::Timeout));

    auto status = mockClient->publishOnTopic(topic, msg, timeoutMs);
    EXPECT_EQ(status, PublishStatus::Timeout);
}
TEST_F(PubSubTest, publishOnTopic_returnsFailsWhenClientDisconnects) {
    const std::string topic = "test/failure";
    const std::string msg = "failure message";
    const int timeoutMs = 1000;

    client->disconnect();
    auto status = client->publishOnTopic(topic, msg, timeoutMs);

    EXPECT_EQ(status, PublishStatus::Failure);
}
TEST_F(PubSubTest, reconnect_reestablishesConnectionSuccessfully) {
    const std::string topic = "test/reconnect";
    const std::string msg = "message after reconnect";
    const int timeoutMs = 2000;

    EXPECT_TRUE(client->isConnected());

    client->disconnect();
    EXPECT_FALSE(client->isConnected());

    EXPECT_NO_THROW(client->reconnect(timeoutMs));
    EXPECT_TRUE(client->isConnected());

    auto sub = client->subscribeTopic(topic);
    sub->onItem([this](auto&& item) {
        receivedMessage(std::forward<decltype(item)>(item));
    });

    client->publishOnTopic(topic, msg, timeoutMs);
    waitForMessage();

    EXPECT_EQ(receivedData, msg);
}
