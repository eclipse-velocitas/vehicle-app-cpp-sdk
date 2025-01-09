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

TEST(Test_AsyncResult, await_withBufferedValue_returnsValueImmediately) {
    AsyncResult<int> asyncResult;
    asyncResult.insertResult(5);
    EXPECT_EQ(5, asyncResult.await());
}

TEST(Test_AsyncResult, await_withoutHandler_blocksCallerUntilResultIsAvailable) {
    constexpr auto INT_RESULT{999};

    AsyncResult<int> asyncResult;
    std::thread      thread([&asyncResult, INT_RESULT]() {
        int temp = INT_RESULT;
        asyncResult.insertResult(std::move(temp));
    });

    auto result = asyncResult.await();
    EXPECT_EQ(INT_RESULT, result);

    thread.join();
}

TEST(Test_AsyncResult, await_withHandler_throws) {
    AsyncResult<int> asyncResult;
    asyncResult.onResult([](int result) {});
    EXPECT_THROW(asyncResult.await(), std::runtime_error);
}

TEST(Test_AsyncResult, onResult_withRegisteredHandler_handlerCalledWithValue) {
    int              receivedResult{-1};
    AsyncResult<int> asyncResult;
    asyncResult.onResult([&receivedResult](int result) { receivedResult = result; });
    asyncResult.insertResult(10);

    EXPECT_EQ(receivedResult, 10);
}

TEST(Test_AsyncResult, onResult_whileAwaiting_throws) {
    AsyncResult<int> asyncResult;
    std::thread      thread([&asyncResult]() { asyncResult.await(); });
    while (!asyncResult.isInAwaitingState()) {
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
    }
    EXPECT_THROW(asyncResult.onResult([](int result) {}), std::runtime_error);
    asyncResult.insertResult(4);
    thread.join();
}
