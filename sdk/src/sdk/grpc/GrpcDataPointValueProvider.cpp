/**
 * Copyright (c) 2022 Robert Bosch GmbH
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

#include "sdk/grpc/GrpcDataPointValueProvider.h"

#include <utility>

namespace velocitas {

GrpcDataPointValueProvider::GrpcDataPointValueProvider(sdv::databroker::v1::Datapoint datapoint)
    : m_datapoint(std::move(datapoint)) {}

const sdv::databroker::v1::Datapoint& GrpcDataPointValueProvider::getDataPoint() const {
    return m_datapoint;
}

bool GrpcDataPointValueProvider::getBoolValue() const { return m_datapoint.bool_value(); }

std::vector<bool> GrpcDataPointValueProvider::getBoolArrayValue() const {
    auto              valueArray = getDataPoint().bool_array().values();
    std::vector<bool> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

float GrpcDataPointValueProvider::getFloatValue() const { return getDataPoint().float_value(); }

std::vector<float> GrpcDataPointValueProvider::getFloatArrayValue() const {
    auto               valueArray = getDataPoint().float_array().values();
    std::vector<float> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

double GrpcDataPointValueProvider::getDoubleValue() const { return getDataPoint().double_value(); }

std::vector<double> GrpcDataPointValueProvider::getDoubleArrayValue() const {
    auto                valueArray = getDataPoint().double_array().values();
    std::vector<double> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

int32_t GrpcDataPointValueProvider::getInt32Value() const { return getDataPoint().int32_value(); }

std::vector<int32_t> GrpcDataPointValueProvider::getInt32ArrayValue() const {
    auto                 valueArray = getDataPoint().uint32_array().values();
    std::vector<int32_t> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

int64_t GrpcDataPointValueProvider::getInt64Value() const { return getDataPoint().int64_value(); }

std::vector<int64_t> GrpcDataPointValueProvider::getInt64ArrayValue() const {
    auto                 valueArray = getDataPoint().int64_array().values();
    std::vector<int64_t> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

uint32_t GrpcDataPointValueProvider::getUint32Value() const {
    return getDataPoint().uint32_value();
}

std::vector<uint32_t> GrpcDataPointValueProvider::getUint32ArrayValue() const {
    auto                  valueArray = getDataPoint().uint32_array().values();
    std::vector<uint32_t> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

uint64_t GrpcDataPointValueProvider::getUint64Value() const {
    return getDataPoint().uint64_value();
}

std::vector<uint64_t> GrpcDataPointValueProvider::getUint64ArrayValue() const {
    auto                  valueArray = getDataPoint().uint64_array().values();
    std::vector<uint64_t> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

std::string GrpcDataPointValueProvider::getStringValue() const {
    return getDataPoint().string_value();
}

std::vector<std::string> GrpcDataPointValueProvider::getStringArrayValue() const {
    auto                     valueArray = getDataPoint().string_array().values();
    std::vector<std::string> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

Timestamp GrpcDataPointValueProvider::getTimestamp() const {
    return {m_datapoint.timestamp().seconds(), m_datapoint.timestamp().nanos()};
}

} // namespace velocitas
