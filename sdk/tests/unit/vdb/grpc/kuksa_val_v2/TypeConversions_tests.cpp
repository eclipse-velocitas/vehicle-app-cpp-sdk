/**
 * Copyright (c) 2024 Contributors to the Eclipse Foundation
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

#include "sdk/vdb/grpc/kuksa_val_v2/TypeConversions.h"

#include "Vehicle.h"
#include "sdk/QueryBuilder.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace velocitas;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::UnorderedElementsAre;

template <typename DATA_TYPE> kuksa::val::v2::Value createGrpcValue(const DATA_TYPE& value) {
    return kuksa::val::v2::Value{};
}

template <> kuksa::val::v2::Value createGrpcValue(const bool& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.set_bool_(value);
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const int8_t& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.set_int32(value);
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const int16_t& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.set_int32(value);
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const int32_t& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.set_int32(value);
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const int64_t& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.set_int64(value);
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const uint8_t& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.set_uint32(value);
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const uint16_t& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.set_uint32(value);
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const uint32_t& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.set_uint32(value);
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const uint64_t& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.set_uint64(value);
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const float& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.set_float_(value);
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const double& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.set_double_(value);
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const std::string& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.set_string(value);
    return grpcValue;
}

template <> kuksa::val::v2::Value createGrpcValue(const std::vector<bool>& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.mutable_bool_array()->mutable_values()->Add(value.cbegin(), value.cend());
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const std::vector<int8_t>& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.mutable_int32_array()->mutable_values()->Add(value.cbegin(), value.cend());
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const std::vector<int16_t>& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.mutable_int32_array()->mutable_values()->Add(value.cbegin(), value.cend());
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const std::vector<int32_t>& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.mutable_int32_array()->mutable_values()->Add(value.cbegin(), value.cend());
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const std::vector<int64_t>& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.mutable_int64_array()->mutable_values()->Add(value.cbegin(), value.cend());
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const std::vector<uint8_t>& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.mutable_uint32_array()->mutable_values()->Add(value.cbegin(), value.cend());
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const std::vector<uint16_t>& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.mutable_uint32_array()->mutable_values()->Add(value.cbegin(), value.cend());
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const std::vector<uint32_t>& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.mutable_uint32_array()->mutable_values()->Add(value.cbegin(), value.cend());
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const std::vector<uint64_t>& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.mutable_uint64_array()->mutable_values()->Add(value.cbegin(), value.cend());
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const std::vector<float>& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.mutable_float_array()->mutable_values()->Add(value.cbegin(), value.cend());
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const std::vector<double>& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.mutable_double_array()->mutable_values()->Add(value.cbegin(), value.cend());
    return grpcValue;
}
template <> kuksa::val::v2::Value createGrpcValue(const std::vector<std::string>& value) {
    kuksa::val::v2::Value grpcValue;
    grpcValue.mutable_string_array()->mutable_values()->Add(value.cbegin(), value.cend());
    return grpcValue;
}

template <typename DATA_TYPE> void testValueConversion(const DATA_TYPE& expectedValue) {
    const auto        grpcValue         = createGrpcValue(expectedValue);
    const auto* const expectedPath      = "some.path";
    const auto        expectedTimestamp = Timestamp{0, 0};

    auto dataPointValue =
        kuksa_val_v2::convertFromGrpcValue(expectedPath, grpcValue, expectedTimestamp);

    ASSERT_TRUE(dataPointValue);
    EXPECT_EQ(expectedPath, dataPointValue->getPath());
    EXPECT_EQ(expectedTimestamp, dataPointValue->getTimestamp());
    EXPECT_TRUE(dataPointValue->isValid());
    EXPECT_EQ(DataPointValue::Failure::NONE, dataPointValue->getFailure());
    EXPECT_TRUE(dataPointValue->wasUpdated());

    auto typedDataPointValue =
        std::dynamic_pointer_cast<TypedDataPointValue<DATA_TYPE>>(dataPointValue);
    ASSERT_NE(nullptr, typedDataPointValue);
    EXPECT_EQ(expectedValue, typedDataPointValue->value());
}

TEST(Test_TypeConversion, convertFromGrpcValue__allMembersSetCorrectly) {
    testValueConversion<bool>(false);
    testValueConversion<bool>(true);
    testValueConversion<int32_t>(std::numeric_limits<int32_t>::min());
    testValueConversion<int32_t>(std::numeric_limits<int32_t>::max());
    testValueConversion<int64_t>(std::numeric_limits<int64_t>::min());
    testValueConversion<int64_t>(std::numeric_limits<int64_t>::max());
    testValueConversion<uint32_t>(std::numeric_limits<uint32_t>::min());
    testValueConversion<uint32_t>(std::numeric_limits<uint32_t>::max());
    testValueConversion<uint64_t>(std::numeric_limits<uint64_t>::min());
    testValueConversion<uint64_t>(std::numeric_limits<uint64_t>::max());
    testValueConversion<float>(std::numeric_limits<float>::min());
    testValueConversion<float>(std::numeric_limits<float>::max());
    testValueConversion<double>(std::numeric_limits<double>::min());
    testValueConversion<double>(std::numeric_limits<double>::max());
    testValueConversion<std::string>("");
    testValueConversion<std::string>(
        "some ridiculous lengthy string to test if a longer string will be properly handled by "
        "this component under test");

    testValueConversion<std::vector<bool>>({false, true, false, true});
    testValueConversion<std::vector<int32_t>>({-100, 0, 42, 123456789});
    testValueConversion<std::vector<int64_t>>({-100, 0, 42, 123456789});
    testValueConversion<std::vector<uint32_t>>({0, 1, 42, 123456789});
    testValueConversion<std::vector<uint64_t>>({0, 1, 42, 123456789});
    testValueConversion<std::vector<float>>({-9.87654321F, 0.0F, 0.1F, 1.23456789F});
    testValueConversion<std::vector<double>>({-9.87654321, 0.0, 0.1, 1.23456789});
    testValueConversion<std::vector<std::string>>({"", "hello", "world", "!"});
}

TEST(Test_TypeConversion, parseQueryIntoRequest_emptyQuery_runtimeError) {
    kuksa::val::v2::SubscribeRequest request;
    EXPECT_THROW(kuksa_val_v2::parseQueryIntoRequest(request, ""), std::runtime_error);
}

TEST(Test_TypeConversion, parseQueryIntoRequest_queryWithoutSelect_runtimeError) {
    kuksa::val::v2::SubscribeRequest request;
    EXPECT_THROW(kuksa_val_v2::parseQueryIntoRequest(request, "Vehicle.Speed"), std::runtime_error);
}

TEST(Test_TypeConversion, parseQueryIntoRequest_queryWithoutSignals_runtimeError) {
    kuksa::val::v2::SubscribeRequest request;
    EXPECT_THROW(kuksa_val_v2::parseQueryIntoRequest(request, "SELECT "), std::runtime_error);
}

TEST(Test_TypeConversion, parseQueryIntoRequest_queryWithSingleSignal_signalPathIsCorrect) {
    const Vehicle     vehicle;
    const std::string query = QueryBuilder::select(vehicle.Speed).build();

    kuksa::val::v2::SubscribeRequest request;
    EXPECT_NO_THROW(kuksa_val_v2::parseQueryIntoRequest(request, query));

    ASSERT_EQ(1, request.signal_paths().size());
    EXPECT_EQ(vehicle.Speed.getPath(), request.signal_paths()[0]);
}

TEST(Test_TypeConversion, parseQueryIntoRequest_queryWithMultipleSignals_signalPathsAreCorrect) {
    Vehicle           vehicle;
    const std::string query =
        QueryBuilder::select({vehicle.Speed, vehicle.Cabin.Seat.Row1.DriverSide.Position,
                              vehicle.Cabin.Seat.Row1.PassengerSide.Position})
            .build();

    kuksa::val::v2::SubscribeRequest request;
    EXPECT_NO_THROW(kuksa_val_v2::parseQueryIntoRequest(request, query));

    ASSERT_EQ(3, request.signal_paths().size());
    EXPECT_EQ(vehicle.Speed.getPath(), request.signal_paths()[0]);
    EXPECT_EQ(vehicle.Cabin.Seat.Row1.DriverSide.Position.getPath(), request.signal_paths()[1]);
    EXPECT_EQ(vehicle.Cabin.Seat.Row1.PassengerSide.Position.getPath(), request.signal_paths()[2]);
}

TEST(Test_TypeConversion, parseQueryIntoRequest_queryWithWhereClause_runtimeError) {
    const Vehicle     vehicle;
    const std::string query =
        QueryBuilder::select(vehicle.Speed).where(vehicle.Speed).gt(30).build();

    kuksa::val::v2::SubscribeRequest request;
    EXPECT_THROW(kuksa_val_v2::parseQueryIntoRequest(request, query), std::runtime_error);
}
