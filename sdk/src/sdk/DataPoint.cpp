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

#include "sdk/DataPoint.h"

#include "sdk/Exceptions.h"

#include <fmt/core.h>

#include <sstream>
#include <utility>

namespace velocitas {

DataPoint::DataPoint(const std::string&                       name,
                     std::shared_ptr<IDataPointValueProvider> valueProvider)
    : Node(name)
    , m_valueProvider(std::move(valueProvider)) {}

const IDataPointValueProvider& DataPoint::getValueProvider() const { return *m_valueProvider; }

DataPointFailure& DataPoint::asFailure() {
    throw InvalidTypeException("DataPoint is not a failure!");
}

Timestamp DataPoint::getTimestamp() const { return m_valueProvider->getTimestamp(); }

DataPointFailure::DataPointFailure(std::string name, std::string failureReason)
    : DataPoint(std::move(name), nullptr)
    , m_failureReason{std::move(failureReason)} {}

std::string DataPointFailure::toString() const {
    return fmt::format("DataPoint: ('{}', failure: '{}')", getName(), getReason());
}

std::string DataPointBoolean::toString() const {
    return fmt::format("DataPointBoolean: ('{}' : '{}')", getName(), value());
}

std::string DataPointBooleanArray::toString() const {
    std::ostringstream oss;
    for (const auto elem : value()) {
        oss << (elem ? "true" : "false") << ",";
    }
    return fmt::format("DataPointBooleanArray: ('{}' : '{}')", getName(), oss.str());
}

std::string DataPointDouble::toString() const {
    return fmt::format("DataPointDouble: ('{}' : '{}')", getName(), value());
}

std::string DataPointDoubleArray::toString() const {
    std::ostringstream oss;
    for (const auto elem : value()) {
        oss << elem << ",";
    }
    return fmt::format("DataPointDoubleArray: ('{}' : '{}')", getName(), oss.str());
}

std::string DataPointFloat::toString() const {
    return fmt::format("DataPointFloat: ('{}' : '{}')", getName(), value());
}

std::string DataPointFloatArray::toString() const {
    std::ostringstream oss;
    for (const auto elem : value()) {
        oss << elem << ",";
    }
    return fmt::format("DataPointFloatArray: ('{}' : '{}')", getName(), oss.str());
}

std::string DataPointInt32::toString() const {
    return fmt::format("DataPointInt32: ('{}' : '{}')", getName(), value());
}

std::string DataPointInt32Array::toString() const {
    std::ostringstream oss;
    for (const auto elem : value()) {
        oss << elem << ",";
    }
    return fmt::format("DataPointInt32Array: ('{}' : '{}')", getName(), oss.str());
}

std::string DataPointInt64::toString() const {
    return fmt::format("DataPointInt64: ('{}' : '{}')", getName(), value());
}

std::string DataPointInt64Array::toString() const {
    std::ostringstream oss;
    for (const auto elem : value()) {
        oss << elem << ",";
    }
    return fmt::format("DataPointInt64Array: ('{}' : '{}')", getName(), oss.str());
}

std::string DataPointUint32::toString() const {
    return fmt::format("DataPointUint32: ('{}' : '{}')", getName(), value());
}

std::string DataPointUint32Array::toString() const {
    std::ostringstream oss;
    for (const auto elem : value()) {
        oss << elem << ",";
    }
    return fmt::format("DataPointUint32Array: ('{}' : '{}')", getName(), oss.str());
}

std::string DataPointUint64::toString() const {
    return fmt::format("DataPointUint64: ('{}' : '{}')", getName(), value());
}

std::string DataPointUint64Array::toString() const {
    std::ostringstream oss;
    for (const auto elem : value()) {
        oss << elem << ",";
    }
    return fmt::format("DataPointUint64Array: ('{}' : '{}')", getName(), oss.str());
}

std::string DataPointString::toString() const {
    return fmt::format("DataPointString: ('{}' : '{}')", getName(), value());
}

std::string DataPointStringArray::toString() const {
    std::ostringstream oss;
    for (const auto elem : value()) {
        oss << elem << ",";
    }
    return fmt::format("DataPointStringArray: ('{}' : '{}')", getName(), oss.str());
}

} // namespace velocitas
