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

#include "sdk/DataPoint.h"
#include "sdk/IPubSubClient.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

class PubSubTest : public ::testing::Test {
protected:
    void SetUp() override {
        client = velocitas::IPubSubClient::createInstance("localhost:1883", "TestClient");
        client->connect();
    }

    void TearDown() override { client->disconnect(); }
    void receivedMessage(const std::string& data) {
        messageReceived = true;
        receivedData    = data;
    }

    void waitForMessage() {
        while (!messageReceived) {
            std::this_thread::sleep_for(std::chrono::seconds{1});
        }
        messageReceived = false;
    }
    std::shared_ptr<velocitas::IPubSubClient> client;
    std::string                               receivedData    = "";
    bool                                      messageReceived = false;
};

TEST_F(PubSubTest, subscribeTopic_publishOnTopic_sameTopic) {
    auto subAbc = client->subscribeTopic("a/b/c");
    subAbc->onItem([this](auto&& item) { receivedMessage(std::forward<decltype(item)>(item)); });
    std::string message = "testMessage";

    client->publishOnTopic("a/b/c", message);
    waitForMessage();
    EXPECT_EQ(receivedData, message);
}
