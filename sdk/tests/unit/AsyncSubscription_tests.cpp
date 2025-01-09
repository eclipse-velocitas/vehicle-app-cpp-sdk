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

#include "sdk/AsyncResult.h"

#include <gtest/gtest.h>

using namespace velocitas;

TEST(Test_AsyncSubcription, next_withBufferedItems_returnsItemsInOrder) {
    AsyncSubscription<int> asyncSubscription;
    asyncSubscription.insertNewItem(1);
    asyncSubscription.insertNewItem(2);
    asyncSubscription.insertNewItem(3);

    EXPECT_EQ(asyncSubscription.next(), 1);
    EXPECT_EQ(asyncSubscription.next(), 2);
    EXPECT_EQ(asyncSubscription.next(), 3);
}

TEST(Test_AsyncSubcription, next_noBufferedItems_blocksUntilItemsInserted) {
    constexpr auto         INT_RESULT{999};
    AsyncSubscription<int> asyncSubscription;
    std::thread            thread([&asyncSubscription, INT_RESULT]() {
        int temp = INT_RESULT;
        asyncSubscription.insertNewItem(std::move(temp));
    });

    EXPECT_EQ(asyncSubscription.next(), INT_RESULT);
    thread.join();
}
