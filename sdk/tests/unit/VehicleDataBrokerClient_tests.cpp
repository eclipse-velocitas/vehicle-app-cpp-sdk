/**
 * Copyright (c) 2022 Robert Bosch GmbH
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

#include "sdk/grpc/VehicleDataBrokerClient.h"

#include <gtest/gtest.h>

#include <cstdlib>

using namespace velocitas;

class Test_VehicleDataBrokerClient : public ::testing::Test {
protected:
    static void SetUpTestSuite() { ::setenv("SDV_MIDDLEWARE_TYPE", "native", /*overwrite=*/true); }

    Test_VehicleDataBrokerClient()
        : m_cut("some-broker-service-name") {}

    VehicleDataBrokerClient m_cut;
};

TEST_F(Test_VehicleDataBrokerClient, getDatapoints_noConnection_throwsAsyncException) {
    EXPECT_THROW(m_cut.getDatapoints({})->await(), AsyncException);
}
