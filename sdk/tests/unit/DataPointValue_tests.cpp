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

#include "sdk/DataPointValue.h"

#include <gtest/gtest.h>
#include <limits>

using namespace velocitas;

class Test_CtorWithAllParamsExplicitly
    : public testing::TestWithParam<
          std::tuple<DataPointValue::Type, std::string, Timestamp, DataPointValue::Failure>> {};

TEST_P(Test_CtorWithAllParamsExplicitly, allMembersAreSetCorrectly) {
    const auto [expectedValueType, expectedPath, expectedTimestamp, expectedValueFailure] =
        GetParam();

    DataPointValue cut{expectedValueType, expectedPath, expectedTimestamp, expectedValueFailure};

    EXPECT_EQ(expectedValueType, cut.getType());
    EXPECT_EQ(expectedPath, cut.getPath());
    EXPECT_EQ(expectedTimestamp, cut.getTimestamp());
    EXPECT_EQ(expectedValueFailure, cut.getFailure());
    if (expectedValueFailure == DataPointValue::Failure::NONE) {
        EXPECT_TRUE(cut.isValid());
    } else {
        EXPECT_FALSE(cut.isValid());
    }
    EXPECT_TRUE(cut.wasUpdated());
    EXPECT_THROW(std::ignore = cut.getValueAsString(), InvalidValueException);
}

INSTANTIATE_TEST_CASE_P(
    Test_DataPointValue, Test_CtorWithAllParamsExplicitly,
    testing::Values(std::make_tuple(DataPointValue::Type::INVALID, "", Timestamp{0, 0},
                                    DataPointValue::Failure::NOT_AVAILABLE),
                    std::make_tuple(DataPointValue::Type::BOOL, "some.path", Timestamp{123, 4567},
                                    DataPointValue::Failure::NONE),
                    std::make_tuple(DataPointValue::Type::STRING_ARRAY, "foo.bar",
                                    Timestamp{987, 6543},
                                    DataPointValue::Failure::INTERNAL_ERROR)));

class Test_CtorWithDefaultFailureValue
    : public testing::TestWithParam<std::tuple<DataPointValue::Type, std::string, Timestamp>> {};

TEST_P(Test_CtorWithDefaultFailureValue, allMembersAreSetCorrectly) {
    const auto [expectedValueType, expectedPath, expectedTimestamp] = GetParam();

    DataPointValue cut{expectedValueType, expectedPath, expectedTimestamp};

    const auto expectedValueFailure = DataPointValue::Failure::NONE;
    EXPECT_EQ(expectedValueType, cut.getType());
    EXPECT_EQ(expectedPath, cut.getPath());
    EXPECT_EQ(expectedTimestamp, cut.getTimestamp());
    EXPECT_EQ(expectedValueFailure, cut.getFailure());
    EXPECT_TRUE(cut.isValid());
    EXPECT_TRUE(cut.wasUpdated());
    EXPECT_THROW(std::ignore = cut.getValueAsString(), InvalidValueException);
}

INSTANTIATE_TEST_CASE_P(
    Test_DataPointValue, Test_CtorWithDefaultFailureValue,
    testing::Values(std::make_tuple(DataPointValue::Type::INVALID, "", Timestamp{0, 0}),
                    std::make_tuple(DataPointValue::Type::BOOL, "some.path", Timestamp{123, 4567}),
                    std::make_tuple(DataPointValue::Type::STRING_ARRAY, "foo.bar",
                                    Timestamp{987, 6543})));

TEST(Test_DataPointValue, clearUpdateStatus__updateStatusIsFalse) {
    const auto        expectedValueType    = DataPointValue::Type::BOOL;
    const auto* const expectedPath         = "some.path";
    const auto        expectedTimestamp    = Timestamp{0, 0};
    const auto        expectedValueFailure = DataPointValue::Failure::ACCESS_DENIED;
    DataPointValue    cut{expectedValueType, expectedPath, expectedTimestamp, expectedValueFailure};

    cut.clearUpdateStatus();

    EXPECT_FALSE(cut.wasUpdated());
    EXPECT_EQ(expectedValueType, cut.getType());
    EXPECT_EQ(expectedPath, cut.getPath());
    EXPECT_EQ(expectedTimestamp, cut.getTimestamp());
    EXPECT_EQ(expectedValueFailure, cut.getFailure());
    if (expectedValueFailure == DataPointValue::Failure::NONE) {
        EXPECT_TRUE(cut.isValid());
    } else {
        EXPECT_FALSE(cut.isValid());
    }
    EXPECT_THROW(std::ignore = cut.getValueAsString(), InvalidValueException);
}

template <typename DATA_TYPE> class TestTypedDataPointValue : public testing::Test {
public:
    DataPointValue::Type valueType() { return getValueType<DATA_TYPE>(); }

    TypedDataPointValue<DATA_TYPE> createTypedDataPointValue() {
        return TypedDataPointValue<DATA_TYPE>();
    };

    TypedDataPointValue<DATA_TYPE> createTypedDataPointValue(const std::string&      path,
                                                             DataPointValue::Failure valueFailure,
                                                             const Timestamp&        timestamp) {
        return TypedDataPointValue<DATA_TYPE>(path, valueFailure, timestamp);
    }
};

using AllDataTypes = ::testing::Types<
    bool, std::vector<bool>, int8_t, std::vector<int8_t>, int16_t, std::vector<int16_t>, int32_t,
    std::vector<int32_t>, int64_t, std::vector<int64_t>, uint8_t, std::vector<uint8_t>, uint16_t,
    std::vector<uint16_t>, uint32_t, std::vector<uint32_t>, uint64_t, std::vector<uint64_t>, float,
    std::vector<float>, double, std::vector<double>, std::string, std::vector<std::string>>;
TYPED_TEST_SUITE(TestTypedDataPointValue, AllDataTypes);

TYPED_TEST(TestTypedDataPointValue, defaultCtor) {
    auto cut = this->createTypedDataPointValue();

    const auto        expectedValueType    = this->valueType();
    const auto* const expectedPath         = "";
    const auto        expectedTimestamp    = Timestamp{0, 0};
    const auto        expectedValueFailure = DataPointValue::Failure::INTERNAL_ERROR;
    EXPECT_EQ(expectedValueType, cut.getType());
    EXPECT_EQ(expectedPath, cut.getPath());
    EXPECT_EQ(expectedTimestamp, cut.getTimestamp());
    EXPECT_EQ(expectedValueFailure, cut.getFailure());
    EXPECT_FALSE(cut.isValid());
    EXPECT_TRUE(cut.wasUpdated());
    EXPECT_NO_THROW(std::ignore = cut.getValueAsString());
}

TYPED_TEST(TestTypedDataPointValue, ctorWithFailure) {
    const auto* const expectedPath         = "foo.bar";
    const auto        expectedTimestamp    = Timestamp{0, 0};
    const auto        expectedValueFailure = DataPointValue::Failure::INTERNAL_ERROR;

    auto cut =
        this->createTypedDataPointValue(expectedPath, expectedValueFailure, expectedTimestamp);

    const auto expectedValueType = this->valueType();
    EXPECT_EQ(expectedValueType, cut.getType());
    EXPECT_EQ(expectedPath, cut.getPath());
    EXPECT_EQ(expectedTimestamp, cut.getTimestamp());
    EXPECT_EQ(expectedValueFailure, cut.getFailure());
    EXPECT_FALSE(cut.isValid());
    EXPECT_TRUE(cut.wasUpdated());
    EXPECT_NO_THROW(std::ignore = cut.getValueAsString());
}

template <typename DATA_TYPE> void checkTypedDataPointValueCtor(const DATA_TYPE& value) {
    const auto        expectedValueType = getValueType<DATA_TYPE>();
    const auto* const expectedPath      = "some.path";
    const auto        expectedTimestamp = Timestamp{1, 2};

    TypedDataPointValue<DATA_TYPE> cut(expectedPath, value, expectedTimestamp);

    EXPECT_EQ(expectedValueType, cut.getType());
    EXPECT_EQ(expectedPath, cut.getPath());
    EXPECT_EQ(expectedTimestamp, cut.getTimestamp());
    EXPECT_EQ(DataPointValue::Failure::NONE, cut.getFailure());
    EXPECT_TRUE(cut.isValid());
    EXPECT_TRUE(cut.wasUpdated());
    EXPECT_NO_THROW(std::ignore = cut.getValueAsString());
}

TEST(Test_TypedDataPointValue, ctorWithValue__allMembersSetCorrectly) {
    checkTypedDataPointValueCtor<bool>(false);
    checkTypedDataPointValueCtor<bool>(true);
    checkTypedDataPointValueCtor<int8_t>(std::numeric_limits<int8_t>::min());
    checkTypedDataPointValueCtor<int8_t>(std::numeric_limits<int8_t>::max());
    checkTypedDataPointValueCtor<int16_t>(std::numeric_limits<int16_t>::min());
    checkTypedDataPointValueCtor<int16_t>(std::numeric_limits<int16_t>::max());
    checkTypedDataPointValueCtor<int32_t>(std::numeric_limits<int32_t>::min());
    checkTypedDataPointValueCtor<int32_t>(std::numeric_limits<int32_t>::max());
    checkTypedDataPointValueCtor<int64_t>(std::numeric_limits<int64_t>::min());
    checkTypedDataPointValueCtor<int64_t>(std::numeric_limits<int64_t>::max());
    checkTypedDataPointValueCtor<uint8_t>(std::numeric_limits<uint8_t>::min());
    checkTypedDataPointValueCtor<uint8_t>(std::numeric_limits<uint8_t>::max());
    checkTypedDataPointValueCtor<uint16_t>(std::numeric_limits<uint16_t>::min());
    checkTypedDataPointValueCtor<uint16_t>(std::numeric_limits<uint16_t>::max());
    checkTypedDataPointValueCtor<uint32_t>(std::numeric_limits<uint32_t>::min());
    checkTypedDataPointValueCtor<uint32_t>(std::numeric_limits<uint32_t>::max());
    checkTypedDataPointValueCtor<uint64_t>(std::numeric_limits<uint64_t>::min());
    checkTypedDataPointValueCtor<uint64_t>(std::numeric_limits<uint64_t>::max());
    checkTypedDataPointValueCtor<float>(std::numeric_limits<float>::min());
    checkTypedDataPointValueCtor<float>(std::numeric_limits<float>::max());
    checkTypedDataPointValueCtor<double>(std::numeric_limits<double>::min());
    checkTypedDataPointValueCtor<double>(std::numeric_limits<double>::max());
    checkTypedDataPointValueCtor<std::string>("");
    checkTypedDataPointValueCtor<std::string>(
        "some ridiculous lengthy string to test if a longer string will be properly handled by "
        "this component under test");

    checkTypedDataPointValueCtor<std::vector<bool>>({false, true, false, true});
    checkTypedDataPointValueCtor<std::vector<int8_t>>({-100, 0, 42, 127});
    checkTypedDataPointValueCtor<std::vector<int16_t>>({-100, 0, 42, 32767});
    checkTypedDataPointValueCtor<std::vector<int32_t>>({-100, 0, 42, 123456789});
    checkTypedDataPointValueCtor<std::vector<int64_t>>({-100, 0, 42, 123456789});
    checkTypedDataPointValueCtor<std::vector<uint8_t>>({0, 1, 42, 255});
    checkTypedDataPointValueCtor<std::vector<uint16_t>>({0, 1, 42, 65535});
    checkTypedDataPointValueCtor<std::vector<uint32_t>>({0, 1, 42, 123456789});
    checkTypedDataPointValueCtor<std::vector<uint64_t>>({0, 1, 42, 123456789});
    checkTypedDataPointValueCtor<std::vector<float>>({-9.87654321F, 0.0F, 0.1F, 1.23456789F});
    checkTypedDataPointValueCtor<std::vector<double>>({-9.87654321, 0.0, 0.1, 1.23456789});
    checkTypedDataPointValueCtor<std::vector<std::string>>({"", "hello", "world", "!"});
}
