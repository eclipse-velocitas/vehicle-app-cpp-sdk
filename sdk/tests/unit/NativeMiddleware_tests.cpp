/**
 * Copyright (c) 2023 Robert Bosch GmbH
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

#include <gtest/gtest.h>

#include <cstdlib>

using namespace velocitas;

class Test_NativeMiddleware : public ::testing::Test {
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

TEST_F(Test_NativeMiddleware, getServiceLocation_envVarNotSet_emptyString) {
    auto serviceLocation = getCut().getServiceLocation("UnknownService");
    EXPECT_EQ("", serviceLocation);
}

TEST_F(Test_NativeMiddleware, getServiceLocation_envVarSet_contentOfEnvVar) {
    ::setenv("SDV_SOMESERVICE_ADDRESS", "some-service-address", /*overwrite=*/true);
    auto serviceLocation = getCut().getServiceLocation("SomeService");
    EXPECT_EQ("some-service-address", serviceLocation);
}

TEST_F(Test_NativeMiddleware, getMetadata__emptyMap) {
    Middleware::Metadata metadata = getCut().getMetadata("don't care");
    EXPECT_TRUE(metadata.empty());
}
