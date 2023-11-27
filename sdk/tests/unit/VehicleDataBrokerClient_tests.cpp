/**
 * Copyright (c) 2022-2023 Contributors to the Eclipse Foundation
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

using namespace velocitas;

TEST(Test_VehicleDataBrokerClient, getDatapoints_noConnection_throwsAsyncException) {
    auto client = VehicleDataBrokerClient("vehicledatabroker");
    EXPECT_THROW(client.getDatapoints({})->await(), AsyncException);
}
