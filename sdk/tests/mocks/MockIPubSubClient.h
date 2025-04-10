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

#ifndef MOCK_IPUBSUBCLIENT_H
#define MOCK_IPUBSUBCLIENT_H

#include "sdk/IPubSubClient.h"
#include <gmock/gmock.h>

namespace velocitas {

class MockIPubSubClient : public IPubSubClient {
public:
    MOCK_METHOD(void, connect, (), (override));
    MOCK_METHOD(void, disconnect, (), (override));
    MOCK_METHOD(void, reconnect, (int timeout_ms), (override));
    MOCK_METHOD(bool, isConnected, (), (const, override));

    MOCK_METHOD(void, publishOnTopic, (const std::string& topic, const std::string& data),
                (override));

    MOCK_METHOD(PublishStatus, publishOnTopic,
                (const std::string& topic, const std::string& data, int timeout_ms), (override));

    MOCK_METHOD(AsyncSubscriptionPtr_t<std::string>, subscribeTopic, (const std::string& topic),
                (override));

    MOCK_METHOD(void, unsubscribeTopic, (const std::string& topic), (override));
};

} // namespace velocitas

#endif // MOCK_IPUBSUBCLIENT_H
