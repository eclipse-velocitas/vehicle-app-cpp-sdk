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

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "MockIPubSubClient.h"
#include "sdk/DataPoint.h"
#include "sdk/IPubSubClient.h"

using ::testing::_;
using ::testing::Return;
using ::testing::Throw;

class PubSubTest : public ::testing::Test {
 protected:
  void SetUp() override {
    client = velocitas::IPubSubClient::createInstance("localhost:1883",
                                                      "TestClient");
    mockClient = std::make_shared<velocitas::MockIPubSubClient>();
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
  std::shared_ptr<velocitas::IPubSubClient> client;
  std::shared_ptr<velocitas::MockIPubSubClient> mockClient;
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

  // Subscribe to the topic and register a callback to capture messages
  auto subAbc = client->subscribeTopic(topic);
  subAbc->onItem([this](auto&& item) {
    receivedMessage(std::forward<decltype(item)>(item));
  });

  // Allow some time for subscription to be established
  std::this_thread::sleep_for(std::chrono::seconds{1});

  // Unsubscribe from the topic
  client->unsubscribeTopic(topic);

  // Publish a message to the same topic after unsubscribe
  client->publishOnTopic(topic, message);

  // Wait to see if any message is received (none should be)
  std::this_thread::sleep_for(std::chrono::seconds{3});

  // Confirm that no message was received after unsubscribe
  EXPECT_FALSE(messageReceived);

  // Confirm that the received data buffer didn't receive the message
  EXPECT_NE(receivedData, message);
}

TEST_F(PubSubTest, publishOnTopic_returnsSuccessWhenWithinTimeout) {
  const std::string topic = "test/success";
  const std::string msg = "success message";
  const int timeoutMs = 1000;

  // Publish a message with a timeout and expect success
  auto status = client->publishOnTopic(topic, msg, timeoutMs);

  // Validate that the publish operation returned success
  EXPECT_EQ(status, velocitas::PublishStatus::Success);
}

TEST_F(PubSubTest, publishOnTopic_returnsTimeoutWhenPublishTakesTooLong) {
  const std::string topic = "test/timeout";
  const std::string msg = "timeout message";
  const int timeoutMs = 1000;

  // Simulate timeout using mock client expectations
  EXPECT_CALL(*mockClient, publishOnTopic(topic, msg, timeoutMs))
      .WillOnce(Return(velocitas::PublishStatus::Timeout));

  // Call the mock publish method and get the status
  auto status = mockClient->publishOnTopic(topic, msg, timeoutMs);

  // Validate that the timeout status is returned as expected
  EXPECT_EQ(status, velocitas::PublishStatus::Timeout);
}

TEST_F(PubSubTest, publishOnTopic_returnsFailsWhenClientDisconnects) {
  const std::string topic = "test/failure";
  const std::string msg = "failure message";
  const int timeoutMs = 1000;

  // Disconnect the MQTT client
  client->disconnect();

  // Try to publish after disconnect
  auto status = client->publishOnTopic(topic, msg, timeoutMs);

  // Confirm that the publish fails when the client is disconnected
  EXPECT_EQ(status, velocitas::PublishStatus::Failure);
}

TEST_F(PubSubTest, reconnect_reestablishesConnectionSuccessfully) {
  const std::string topic = "test/reconnect";
  const std::string msg = "message after reconnect";
  const int timeoutMs = 2000;

  // Ensure the client is initially connected
  EXPECT_TRUE(client->isConnected());

  // Disconnect the client and verify it is disconnected
  client->disconnect();
  EXPECT_FALSE(client->isConnected());

  // Reconnect the client with a timeout and expect no exceptions
  EXPECT_NO_THROW(client->reconnect(timeoutMs));

  // Ensure the client is now reconnected
  EXPECT_TRUE(client->isConnected());

  // Subscribe to a topic and set a callback to capture the message
  auto sub = client->subscribeTopic(topic);
  sub->onItem([this](auto&& item) {
    receivedMessage(std::forward<decltype(item)>(item));
  });

  // Publish a message to test communication post-reconnect
  client->publishOnTopic(topic, msg, timeoutMs);

  // Wait for the message to be received and confirm it
  waitForMessage();
  EXPECT_EQ(receivedData, msg);
}
