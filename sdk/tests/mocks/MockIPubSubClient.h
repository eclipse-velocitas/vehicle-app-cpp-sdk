#ifndef MOCK_IPUBSUBCLIENT_H
#define MOCK_IPUBSUBCLIENT_H

#include "sdk/IPubSubClient.h"
#include <gmock/gmock.h>

namespace velocitas {

class MockIPubSubClient : public IPubSubClient {
public:
    MOCK_METHOD(void, connect, (), (override));
    MOCK_METHOD(void, disconnect, (), (override));
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
