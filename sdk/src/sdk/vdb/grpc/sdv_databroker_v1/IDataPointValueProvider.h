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

#ifndef VEHICLE_APP_SDK_IDATAPOINTVALUEPROVIDER_H
#define VEHICLE_APP_SDK_IDATAPOINTVALUEPROVIDER_H

#include "sdk/DataPointValue.h"

#include <cstdint>
#include <string>
#include <vector>

namespace velocitas::sdv_databroker_v1 {

/**
 * @brief Interface for providing values to Datapoints.
 *        Allows different communication middleware to feed results into
 *        DataPoints.
 */
class IDataPointValueProvider {
public:
    IDataPointValueProvider()          = default;
    virtual ~IDataPointValueProvider() = default;

    IDataPointValueProvider(const IDataPointValueProvider&)            = delete;
    IDataPointValueProvider(IDataPointValueProvider&&)                 = delete;
    IDataPointValueProvider& operator=(const IDataPointValueProvider&) = delete;
    IDataPointValueProvider& operator=(IDataPointValueProvider&&)      = delete;

    [[nodiscard]] virtual DataPointValue::Failure getFailure() const = 0;

    [[nodiscard]] virtual bool getBoolValue() const = 0;

    [[nodiscard]] virtual std::vector<bool> getBoolArrayValue() const = 0;

    [[nodiscard]] virtual float getFloatValue() const = 0;

    [[nodiscard]] virtual std::vector<float> getFloatArrayValue() const = 0;

    [[nodiscard]] virtual double getDoubleValue() const = 0;

    [[nodiscard]] virtual std::vector<double> getDoubleArrayValue() const = 0;

    [[nodiscard]] virtual int8_t getInt8Value() const = 0;

    [[nodiscard]] virtual std::vector<int8_t> getInt8ArrayValue() const = 0;

    [[nodiscard]] virtual int16_t getInt16Value() const = 0;

    [[nodiscard]] virtual std::vector<int16_t> getInt16ArrayValue() const = 0;

    [[nodiscard]] virtual int32_t getInt32Value() const = 0;

    [[nodiscard]] virtual std::vector<int32_t> getInt32ArrayValue() const = 0;

    [[nodiscard]] virtual int64_t getInt64Value() const = 0;

    [[nodiscard]] virtual std::vector<int64_t> getInt64ArrayValue() const = 0;

    [[nodiscard]] virtual uint8_t getUint8Value() const = 0;

    [[nodiscard]] virtual std::vector<uint8_t> getUint8ArrayValue() const = 0;

    [[nodiscard]] virtual uint16_t getUint16Value() const = 0;

    [[nodiscard]] virtual std::vector<uint16_t> getUint16ArrayValue() const = 0;

    [[nodiscard]] virtual uint32_t getUint32Value() const = 0;

    [[nodiscard]] virtual std::vector<uint32_t> getUint32ArrayValue() const = 0;

    [[nodiscard]] virtual uint64_t getUint64Value() const = 0;

    [[nodiscard]] virtual std::vector<uint64_t> getUint64ArrayValue() const = 0;

    [[nodiscard]] virtual std::string getStringValue() const = 0;

    [[nodiscard]] virtual std::vector<std::string> getStringArrayValue() const = 0;
};

} // namespace velocitas::sdv_databroker_v1

#endif // VEHICLE_APP_SDK_IDATAPOINTVALUEPROVIDER_H
