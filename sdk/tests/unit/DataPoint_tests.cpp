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

#include "sdk/DataPoint.h"
#include "sdk/DataPointValue.h"
#include "sdk/VehicleModelContext.h"

#include "VehicleDataBrokerClientMock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace velocitas;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::UnorderedElementsAre;

template <typename T> void setTestCaseImpl(typename T::value_type value) {
    const std::string MY_PATH{"foo.bar"};

    auto mockVdbc    = std::make_shared<VehicleDataBrokerClientMock>();
    auto asyncResult = std::make_shared<AsyncResult<IVehicleDataBrokerClient::SetErrorMap_t>>();
    asyncResult->insertResult(
        IVehicleDataBrokerClient::SetErrorMap_t{}); // pre-fill result so we don't block

    auto needle = TypedDataPointValue<typename T::value_type>(MY_PATH, value);
    EXPECT_CALL(*mockVdbc, setDatapoints(UnorderedElementsAre(Pointee(needle))))
        .Times(1)
        .WillRepeatedly(Return(asyncResult));

    VehicleModelContext::getInstance().setVdbc(mockVdbc);

    T          myDataPoint{MY_PATH, nullptr};
    const auto status = myDataPoint.set(value)->await();

    EXPECT_TRUE(status.ok());
}

template <typename T> void getTestCaseImpl(typename T::value_type expectedValue) {
    const std::string MY_PATH{"foo.bar"};

    auto mockVdbc    = std::make_shared<VehicleDataBrokerClientMock>();
    auto asyncResult = std::make_shared<AsyncResult<DataPointReply>>();

    DataPointMap_t map;
    map[MY_PATH] =
        std::make_shared<TypedDataPointValue<typename T::value_type>>(MY_PATH, expectedValue);
    asyncResult->insertResult(DataPointReply(std::move(map))); // pre-fill result so we don't block

    EXPECT_CALL(*mockVdbc, getDatapoints(UnorderedElementsAre(MY_PATH)))
        .Times(1)
        .WillRepeatedly(Return(asyncResult));

    VehicleModelContext::getInstance().setVdbc(mockVdbc);

    T          myDataPoint{MY_PATH, nullptr};
    const auto returnedValue = myDataPoint.get()->await();

    EXPECT_EQ(expectedValue, returnedValue.value());
}

TEST(Test_DataPoint, set_allDatatypes_forwardedToVdbc) {
    setTestCaseImpl<DataPointBoolean>(false);
    setTestCaseImpl<DataPointBooleanArray>({true, false, true});

    setTestCaseImpl<DataPointInt8>(-8); // NOLINT
    setTestCaseImpl<DataPointInt8Array>({-1, -2, -3, -4});

    setTestCaseImpl<DataPointInt16>(-16); // NOLINT
    setTestCaseImpl<DataPointInt16Array>({-1, -2, -3, -4});

    setTestCaseImpl<DataPointInt32>(-32); // NOLINT
    setTestCaseImpl<DataPointInt32Array>({-1, -2, -3, -4});

    setTestCaseImpl<DataPointInt64>(-128); // NOLINT
    setTestCaseImpl<DataPointInt64Array>({-1, -2, -3, -4});

    setTestCaseImpl<DataPointUint8>(8u); // NOLINT
    setTestCaseImpl<DataPointUint8Array>({1U, 2U, 3U, 4U});

    setTestCaseImpl<DataPointUint16>(16u); // NOLINT
    setTestCaseImpl<DataPointUint16Array>({1U, 2U, 3U, 4U});

    setTestCaseImpl<DataPointUint32>(32u); // NOLINT
    setTestCaseImpl<DataPointUint32Array>({1U, 2U, 3U, 4U});

    setTestCaseImpl<DataPointUint64>(128u); // NOLINT
    setTestCaseImpl<DataPointUint64Array>({1U, 2U, 3U, 4U});

    setTestCaseImpl<DataPointFloat>(200.0F);                        // NOLINT
    setTestCaseImpl<DataPointFloatArray>({1.0F, 2.0F, 3.0F, 4.0F}); // NOLINT

    setTestCaseImpl<DataPointDouble>(200.0L);                        // NOLINT
    setTestCaseImpl<DataPointDoubleArray>({1.0L, 2.0L, 3.0L, 4.0L}); // NOLINT

    setTestCaseImpl<DataPointString>("foo");                      // NOLINT
    setTestCaseImpl<DataPointStringArray>({"foo", "bar", "baz"}); // NOLINT
}

TEST(Test_DataPoint, get_allDataTypes_forwardedToVdbc) {
    getTestCaseImpl<DataPointBoolean>(false);
    getTestCaseImpl<DataPointBooleanArray>({true, false, true});

    getTestCaseImpl<DataPointInt32>(32); // NOLINT
    getTestCaseImpl<DataPointInt32Array>({1, 2, 3, 4});

    getTestCaseImpl<DataPointInt64>(128); // NOLINT
    getTestCaseImpl<DataPointInt64Array>({1, 2, 3, 4});

    getTestCaseImpl<DataPointUint32>(32u); // NOLINT
    getTestCaseImpl<DataPointUint32Array>({1U, 2U, 3U, 4U});

    getTestCaseImpl<DataPointUint64>(128u); // NOLINT
    getTestCaseImpl<DataPointUint64Array>({1U, 2U, 3U, 4U});

    getTestCaseImpl<DataPointFloat>(200.0F);                        // NOLINT
    getTestCaseImpl<DataPointFloatArray>({1.0F, 2.0F, 3.0F, 4.0F}); // NOLINT

    getTestCaseImpl<DataPointDouble>(200.0L);                        // NOLINT
    getTestCaseImpl<DataPointDoubleArray>({1.0L, 2.0L, 3.0L, 4.0L}); // NOLINT

    getTestCaseImpl<DataPointString>("foo");                      // NOLINT
    getTestCaseImpl<DataPointStringArray>({"foo", "bar", "baz"}); // NOLINT
}
