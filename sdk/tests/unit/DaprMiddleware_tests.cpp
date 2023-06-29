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

#include "sdk/middleware/DaprMiddleware.h"

#include <gtest/gtest.h>

#include <cstdlib>

using namespace velocitas;

class Test_DaprMiddleware : public ::testing::Test {
protected:
    Middleware& getCut() { return m_cut; }

private:
    DaprMiddleware m_cut;
};

TEST_F(Test_DaprMiddleware, getTypeId__typeIdIsDapr) {
    ::std::string typeId = getCut().getTypeId();
    EXPECT_EQ(DaprMiddleware::TYPE_ID, typeId);
}

TEST_F(Test_DaprMiddleware, createPubSubClient__validPointer) {
    std::shared_ptr<IPubSubClient> pubSubClient;
    EXPECT_NO_THROW(pubSubClient = getCut().createPubSubClient("My Test Id"));
    EXPECT_NE(nullptr, pubSubClient.get());
}

TEST_F(Test_DaprMiddleware, getServiceLocation_envDaprGrpcPortSet_contentOfEnvVar) {
    ::setenv("DAPR_GRPC_PORT", "12345", /*overwrite=*/true);
    auto serviceLocation = getCut().getServiceLocation("SomeService");
    EXPECT_EQ("localhost:12345", serviceLocation);
}

TEST_F(Test_DaprMiddleware, getServiceLocation_envDaprGrpcPortNotSet_throwsRuntimeError) {
    ::unsetenv("DAPR_GRPC_PORT");
    EXPECT_THROW(getCut().getServiceLocation("UnknownService"), std::runtime_error);
}

TEST_F(Test_DaprMiddleware, getMetadata_envVarWithApppIdSet_metadataWithContentOfEnvVar) {
    ::setenv("SOMESERVICE_DAPR_APP_ID", "some-explicit-app-id", /*overwrite=*/true);
    Middleware::Metadata metadata         = getCut().getMetadata("SomeService");
    Middleware::Metadata expectedMetadata = {{"dapr-app-id", "some-explicit-app-id"}};
    EXPECT_EQ(expectedMetadata, metadata);
}

TEST_F(Test_DaprMiddleware, getMetadata_envVarWithApppIdNotSet_metadataWithDefaultAppId) {
    ::unsetenv("SOMESERVICE_DAPR_APP_ID");
    Middleware::Metadata metadata         = getCut().getMetadata("SomeService");
    Middleware::Metadata expectedMetadata = {{"dapr-app-id", "someservice"}};
    EXPECT_EQ(expectedMetadata, metadata);
}
