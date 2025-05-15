/**
 * Copyright (c) 2024-2025 Contributors to the Eclipse Foundation
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

#include "sdk/grpc/GrpcCall.h"
#include "sdk/grpc/GrpcClient.h"

#include <gtest/gtest.h>

using namespace velocitas;

TEST(Test_GrpcClient, addActiveCall_newlyCreatedGrpcClient_oneActiveCall) {
    // preparation
    GrpcClient cut;

    // test
    cut.addActiveCall(std::make_shared<GrpcCall>());
    EXPECT_EQ(1, cut.getNumActiveCalls());
}

TEST(Test_GrpcClient, addActiveCall_oneActiveCallPresent_twoActiveCalls) {
    // preparation
    GrpcClient cut;
    cut.addActiveCall(std::make_shared<GrpcCall>());

    // test
    cut.addActiveCall(std::make_shared<GrpcCall>());
    EXPECT_EQ(2, cut.getNumActiveCalls());
}

TEST(Test_GrpcClient, addActiveCall_oneFinishedCallPresent_oneActiveCall) {
    // preparation
    GrpcClient cut;
    auto       finishedCall = std::make_shared<GrpcCall>();
    cut.addActiveCall(finishedCall);
    finishedCall->m_isComplete = true;

    // test
    cut.addActiveCall(std::make_shared<GrpcCall>());
    EXPECT_EQ(1, cut.getNumActiveCalls());
}

TEST(Test_GrpcClient, addActiveCall_oneFinishedAndOneActiveCallPresent_finishedCallRemoved) {
    // preparation
    GrpcClient cut;
    auto       finishedCall = std::make_shared<GrpcCall>();
    cut.addActiveCall(finishedCall);
    auto activeCall = std::make_shared<GrpcCall>();
    cut.addActiveCall(activeCall);
    finishedCall->m_isComplete = true;

    // test
    auto anotherActiveCall = std::make_shared<GrpcCall>();
    cut.addActiveCall(anotherActiveCall);
    EXPECT_EQ(2, cut.getNumActiveCalls());
    EXPECT_EQ(1, finishedCall.use_count()); // no other reference held to this pointer
    EXPECT_EQ(2, activeCall.use_count());
    EXPECT_EQ(2, anotherActiveCall.use_count());
}
