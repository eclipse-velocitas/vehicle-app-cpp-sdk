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
#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <fmt/core.h>

#include <memory>
#include <sstream>
#include <utility>

namespace velocitas {

DataPoint::DataPoint(const std::string& name)
    : Node(name) {}

DataPointFailure::DataPointFailure(std::string name, std::string failureReason)
    : DataPoint(std::move(name), nullptr)
    , m_failureReason{std::move(failureReason)} {}

std::string DataPointFailure::toString() const {
    return fmt::format("DataPoint: ('{}', failure: '{}')", getName(), getReason());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointBoolean::value_type>> DataPointBoolean::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<value_type>>([this](const DataPointsResult& dataPointsResult) {
            return *dataPointsResult.get(*this);
        });
}

AsyncResultPtr_t<Status> DataPointBoolean::set(DataPointBoolean::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointBoolean::toString() const {
    return fmt::format("DataPointBoolean: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointBooleanArray::value_type>>
DataPointBooleanArray::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointBooleanArray::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointBooleanArray::set(DataPointBooleanArray::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointBooleanArray::toString() const {
    return fmt::format("DataPointBooleanArray: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointDouble::value_type>> DataPointDouble::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointDouble::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointDouble::set(DataPointDouble::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointDouble::toString() const {
    return fmt::format("DataPointDouble: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointDoubleArray::value_type>>
DataPointDoubleArray::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointDoubleArray::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointDoubleArray::set(DataPointDoubleArray::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointDoubleArray::toString() const {
    return fmt::format("DataPointDoubleArray: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointFloat::value_type>> DataPointFloat::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointFloat::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointFloat::set(DataPointFloat::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointFloat::toString() const {
    return fmt::format("DataPointFloat: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointFloatArray::value_type>>
DataPointFloatArray::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointFloatArray::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointFloatArray::set(DataPointFloatArray::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointFloatArray::toString() const {
    return fmt::format("DataPointFloatArray: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointInt32::value_type>> DataPointInt32::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointInt32::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointInt32::set(DataPointInt32::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointInt32::toString() const {
    return fmt::format("DataPointInt32: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointInt32Array::value_type>>
DataPointInt32Array::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointInt32Array::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointInt32Array::set(DataPointInt32Array::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointInt32Array::toString() const {
    return fmt::format("DataPointInt32Array: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointInt64::value_type>> DataPointInt64::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointInt64::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointInt64::set(DataPointInt64::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointInt64::toString() const {
    return fmt::format("DataPointInt64: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointInt64Array::value_type>>
DataPointInt64Array::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointInt64Array::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointInt64Array::set(DataPointInt64Array::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointInt64Array::toString() const {
    return fmt::format("DataPointInt64Array: ('{}' : '{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointUint32::value_type>> DataPointUint32::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointUint32::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointUint32::set(DataPointUint32::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointUint32::toString() const {
    return fmt::format("DataPointUint32: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointUint32Array::value_type>>
DataPointUint32Array::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointUint32Array::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointUint32Array::set(DataPointUint32Array::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointUint32Array::toString() const {
    return fmt::format("DataPointUint32Array: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointUint64::value_type>> DataPointUint64::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointUint64::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointUint64::set(DataPointUint64::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointUint64::toString() const {
    return fmt::format("DataPointUint64: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointUint64Array::value_type>>
DataPointUint64Array::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointUint64Array::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointUint64Array::set(DataPointUint64Array::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointUint64Array::toString() const {
    return fmt::format("DataPointUint64Array: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointString::value_type>> DataPointString::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointString::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointString::set(DataPointString::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointString::toString() const {
    return fmt::format("DataPointString: ('{}')", getName());
}

AsyncResultPtr_t<TypedDataPointResult<DataPointStringArray::value_type>>
DataPointStringArray::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointResult<DataPointStringArray::value_type>>(
            [this](const DataPointsResult& dataPointsResult) {
                return *dataPointsResult.get(*this);
            });
}

AsyncResultPtr_t<Status> DataPointStringArray::set(DataPointStringArray::value_type value) {
    std::vector<std::unique_ptr<DataPointResult>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointResult<value_type>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

std::string DataPointStringArray::toString() const {
    return fmt::format("DataPointStringArray: ('{}')", getName());
}

} // namespace velocitas
