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

#include "sdk/QueryBuilder.h"

#include <gtest/gtest.h>

using namespace velocitas;

TEST(Test_QueryBuilder, select_singleDataPoint) {
    DataPointFloat foo{"foo", nullptr};
    const auto     query = QueryBuilder::select(foo).build();
    ASSERT_EQ(query, "SELECT foo");
}

TEST(Test_QueryBuilder, select_multipleDataPoints) {
    DataPointFloat foo{"foo", nullptr};
    DataPointFloat bar{"bar", nullptr};
    const auto     query = QueryBuilder::select({foo, bar}).build();
    ASSERT_EQ(query, "SELECT foo, bar");
}

TEST(Test_QueryBuilder, whereCondition_gt) {
    DataPointFloat foo{"foo", nullptr};
    const auto     query = QueryBuilder::select(foo).where(foo).gt(10.0F).build();
    ASSERT_EQ(query, "SELECT foo WHERE foo > 10.000000");
}

TEST(Test_QueryBuilder, whereCondition_lt) {
    DataPointInt32 foo{"foo", nullptr};
    const auto     query = QueryBuilder::select(foo).where(foo).lt(100).build();
    ASSERT_EQ(query, "SELECT foo WHERE foo < 100");
}

TEST(Test_QueryBuilder, whereCondition_eq) {
    DataPointBoolean foo{"foo", nullptr};
    const auto       query = QueryBuilder::select(foo).where(foo).eq(true).build();
    ASSERT_EQ(query, "SELECT foo WHERE foo = 1");
}
