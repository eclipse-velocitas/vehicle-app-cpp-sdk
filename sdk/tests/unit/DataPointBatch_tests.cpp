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

#include "sdk/DataPointBatch.h"

#include "sdk/DataPoint.h"
#include "sdk/DataPointValue.h"
#include "sdk/Exceptions.h"
#include "sdk/VehicleModelContext.h"

#include "VehicleDataBrokerClientMock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace velocitas;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::UnorderedElementsAre;

class Test_DataPointBatch : public ::testing::Test {
public:
    void SetUp() override {
        m_vdbcMock    = std::make_shared<VehicleDataBrokerClientMock>();
        m_asyncResult = std::make_shared<AsyncResult<IVehicleDataBrokerClient::SetErrorMap_t>>();
        m_asyncResult->insertResult(
            IVehicleDataBrokerClient::SetErrorMap_t{}); // pre-fill result so we don't block

        VehicleModelContext::getInstance().setVdbc(m_vdbcMock);
    }

    template <typename T> void bla(const std::string& path, typename T::value_type value) {
        auto needle = TypedDataPointValue<typename T::value_type>(path, value);
        EXPECT_CALL(*m_vdbcMock, setDatapoints(UnorderedElementsAre(Pointee(needle))))
            .Times(1)
            .WillRepeatedly(Return(m_asyncResult));
    }

protected:
    std::shared_ptr<VehicleDataBrokerClientMock>                          m_vdbcMock;
    std::shared_ptr<AsyncResult<IVehicleDataBrokerClient::SetErrorMap_t>> m_asyncResult;
};

TEST_F(Test_DataPointBatch, apply_noPoints_throwException) {
    DataPointBatch batch;
    EXPECT_THROW(batch.apply(), InvalidValueException);
}

TEST_F(Test_DataPointBatch, apply_onePoint_requestContainsOnePoint) {
    DataPointInt32 dataPoint{"foo", nullptr};

    EXPECT_CALL(*m_vdbcMock, setDatapoints(UnorderedElementsAre(Pointee(
                                 TypedDataPointValue<DataPointInt32::value_type>("foo", 1)))))
        .Times(1)
        .WillRepeatedly(Return(m_asyncResult));

    DataPointBatch batch;
    batch.add(dataPoint, 1);
    const auto errorMap = batch.apply()->await();
    EXPECT_TRUE(errorMap.empty());
}

TEST_F(Test_DataPointBatch, apply_twoPoints_requestContainsTwoPoints) {
    const std::string path1{"foo"};
    const std::string path2{"bar"};
    const int32_t     value1 = 100;
    const float       value2 = 9412.F;

    EXPECT_CALL(*m_vdbcMock,
                setDatapoints(UnorderedElementsAre(
                    Pointee(TypedDataPointValue<DataPointInt32::value_type>(path1, value1)),
                    Pointee(TypedDataPointValue<DataPointFloat::value_type>(path2, value2)))))
        .Times(1)
        .WillRepeatedly(Return(m_asyncResult));

    DataPointInt32 dataPoint1{path1, nullptr};
    DataPointFloat dataPoint2{path2, nullptr};

    DataPointBatch batch;
    batch.add(dataPoint1, value1);
    batch.add(dataPoint2, value2);
    const auto errorMap = batch.apply()->await();
    EXPECT_TRUE(errorMap.empty());
}

TEST_F(Test_DataPointBatch, apply_threePoints_requestContainsThreePoints) {
    const std::string path1{"foo"};
    const std::string path2{"bar"};
    const std::string path3{"baz"};
    const int32_t     value1 = 100;
    const float       value2 = 9412.F;
    const bool        value3 = false;

    EXPECT_CALL(*m_vdbcMock,
                setDatapoints(UnorderedElementsAre(
                    Pointee(TypedDataPointValue<DataPointInt32::value_type>(path1, value1)),
                    Pointee(TypedDataPointValue<DataPointFloat::value_type>(path2, value2)),
                    Pointee(TypedDataPointValue<DataPointBoolean::value_type>(path3, value3)))))
        .Times(1)
        .WillRepeatedly(Return(m_asyncResult));

    DataPointInt32   dataPoint1{path1, nullptr};
    DataPointFloat   dataPoint2{path2, nullptr};
    DataPointBoolean dataPoint3{path3, nullptr};

    DataPointBatch batch;
    batch.add(dataPoint1, value1);
    batch.add(dataPoint2, value2);
    batch.add(dataPoint3, value3);
    const auto errorMap = batch.apply()->await();
    EXPECT_TRUE(errorMap.empty());
}
