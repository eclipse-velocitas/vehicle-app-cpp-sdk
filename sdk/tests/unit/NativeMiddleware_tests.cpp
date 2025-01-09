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

#include "sdk/middleware/NativeMiddleware.h"

#include "TestBaseUsingEnvVars.h"
#include <gtest/gtest.h>

using namespace velocitas;

class Test_NativeMiddleware : public TestUsingEnvVars {
protected:
    Middleware& getCut() { return m_cut; }

private:
    NativeMiddleware m_cut;
};

TEST_F(Test_NativeMiddleware, getTypeId__typeIdIsNative) {
    ::std::string typeId = getCut().getTypeId();
    EXPECT_EQ(NativeMiddleware::TYPE_ID, typeId);
}

TEST_F(Test_NativeMiddleware, createPubSubClient__validPointer) {
    std::shared_ptr<IPubSubClient> pubSubClient;
    EXPECT_NO_THROW(pubSubClient = getCut().createPubSubClient("My Test Id"));
    EXPECT_NE(nullptr, pubSubClient.get());
}

TEST_F(Test_NativeMiddleware, getServiceLocation_envVarNotSet_throwsRuntimeError) {
    EXPECT_THROW(getCut().getServiceLocation("UnknownService"), std::runtime_error);
}

TEST_F(Test_NativeMiddleware, getServiceLocation_envVarNotSetButDefaultKnown_default) {
    auto serviceLocation = getCut().getServiceLocation("mqtt");
    EXPECT_EQ("localhost:1883", serviceLocation);
}

TEST_F(Test_NativeMiddleware, getServiceLocation_envVarSetWithPureAddress_contentOfEnvVar) {
    setEnvVar("SDV_SOMESERVICE_ADDRESS", "some-service-address");
    auto serviceLocation = getCut().getServiceLocation("SomeService");
    EXPECT_EQ("some-service-address", serviceLocation);
}

TEST_F(Test_NativeMiddleware, getServiceLocation_envVarSetWithUrl_contentOfUrlsNetLocation) {
    setEnvVar("SDV_SOMESERVICE_ADDRESS", "scheme://some-host:port/path");
    auto serviceLocation = getCut().getServiceLocation("SomeService");
    EXPECT_EQ("some-host:port", serviceLocation);
}

TEST_F(Test_NativeMiddleware, getMetadata__emptyMap) {
    Middleware::Metadata metadata = getCut().getMetadata("don't care");
    EXPECT_TRUE(metadata.empty());
}
