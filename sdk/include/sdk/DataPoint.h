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

#ifndef VEHICLE_APP_SDK_DATAPOINT_H
#define VEHICLE_APP_SDK_DATAPOINT_H

#include "sdk/Node.h"

#include <memory>
#include <string>
#include <vector>

namespace velocitas {

/**
 * @brief See
 * https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/timestamp.proto
 */
struct Timestamp {
    int64_t seconds;
    int32_t nanos;
};

class DataPoint;
class DataPointBoolean;
class DataPointBooleanArray;
class DataPointDouble;
class DataPointDoubleArray;
class DataPointFloat;
class DataPointFloatArray;
class DataPointDouble;
class DataPointDoubleArray;
class DataPointInt32;
class DataPointInt32Array;
class DataPointInt64;
class DataPointInt64Array;
class DataPointUint32;
class DataPointUint32Array;
class DataPointUint64;
class DataPointUint64Array;
class DataPointString;
class DataPointStringArray;
class DataPointFailure;

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

    [[nodiscard]] virtual bool getBoolValue() const = 0;

    [[nodiscard]] virtual std::vector<bool> getBoolArrayValue() const = 0;

    [[nodiscard]] virtual float getFloatValue() const = 0;

    [[nodiscard]] virtual std::vector<float> getFloatArrayValue() const = 0;

    [[nodiscard]] virtual double getDoubleValue() const = 0;

    [[nodiscard]] virtual std::vector<double> getDoubleArrayValue() const = 0;

    [[nodiscard]] virtual int32_t getInt32Value() const = 0;

    [[nodiscard]] virtual std::vector<int32_t> getInt32ArrayValue() const = 0;

    [[nodiscard]] virtual int64_t getInt64Value() const = 0;

    [[nodiscard]] virtual std::vector<int64_t> getInt64ArrayValue() const = 0;

    [[nodiscard]] virtual uint32_t getUint32Value() const = 0;

    [[nodiscard]] virtual std::vector<uint32_t> getUint32ArrayValue() const = 0;

    [[nodiscard]] virtual uint64_t getUint64Value() const = 0;

    [[nodiscard]] virtual std::vector<uint64_t> getUint64ArrayValue() const = 0;

    [[nodiscard]] virtual std::string getStringValue() const = 0;

    [[nodiscard]] virtual std::vector<std::string> getStringArrayValue() const = 0;

    [[nodiscard]] virtual Timestamp getTimestamp() const = 0;
};

/**
 * @brief Base class for data points.
 *        Do not use for modelling directly.
 *
 * @details Provides as*Type*() methods to convert the base data point into any of its derived
 * types. If the type is not the one you are treating it as (i.e. using asInt32() on a bool type) an
 * InvalidTypeException is thrown at runtime.
 */
class DataPoint : public Node {
public:
    using Node::Node;

    DataPoint(const std::string& name, std::shared_ptr<IDataPointValueProvider> valueProvider);

    ~DataPoint() override = default;

    DataPoint(const DataPoint&)            = delete;
    DataPoint(DataPoint&&)                 = delete;
    DataPoint& operator=(const DataPoint&) = delete;
    DataPoint& operator=(DataPoint&&)      = delete;

    virtual DataPointFailure&         asFailure();
    [[nodiscard]] virtual bool        isValid() const { return true; }
    [[nodiscard]] virtual std::string toString() const = 0;

    /**
     * @brief Return the timestamp of the data point.
     *
     * @return Timestamp The timestamp at which the value was captured.
     */
    [[nodiscard]] Timestamp getTimestamp() const;

protected:
    [[nodiscard]] const IDataPointValueProvider& getValueProvider() const;

private:
    std::shared_ptr<IDataPointValueProvider> m_valueProvider;
};

/**
 * @brief A data point with the type bool.
 *
 */
class DataPointBoolean : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = bool;

    ~DataPointBoolean() override = default;

    DataPointBoolean(const DataPointBoolean&)            = delete;
    DataPointBoolean(DataPointBoolean&&)                 = delete;
    DataPointBoolean& operator=(const DataPointBoolean&) = delete;
    DataPointBoolean& operator=(DataPointBoolean&&)      = delete;

    [[nodiscard]] bool value() const { return getValueProvider().getBoolValue(); }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type bool array.
 *
 */
class DataPointBooleanArray : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = std::vector<bool>;

    ~DataPointBooleanArray() override = default;

    DataPointBooleanArray(const DataPointBooleanArray&)            = delete;
    DataPointBooleanArray(DataPointBooleanArray&&)                 = delete;
    DataPointBooleanArray& operator=(const DataPointBooleanArray&) = delete;
    DataPointBooleanArray& operator=(DataPointBooleanArray&&)      = delete;

    [[nodiscard]] std::vector<bool> value() const { return getValueProvider().getBoolArrayValue(); }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type int32.
 *
 */
class DataPointInt32 : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = int32_t;

    ~DataPointInt32() override = default;

    DataPointInt32(const DataPointInt32&)            = delete;
    DataPointInt32(DataPointInt32&&)                 = delete;
    DataPointInt32& operator=(const DataPointInt32&) = delete;
    DataPointInt32& operator=(DataPointInt32&&)      = delete;

    [[nodiscard]] int32_t value() const { return getValueProvider().getInt32Value(); }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type int32 array.
 *
 */
class DataPointInt32Array : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = std::vector<int32_t>;

    ~DataPointInt32Array() override = default;

    DataPointInt32Array(const DataPointInt32Array&)            = delete;
    DataPointInt32Array(DataPointInt32Array&&)                 = delete;
    DataPointInt32Array& operator=(const DataPointInt32Array&) = delete;
    DataPointInt32Array& operator=(DataPointInt32Array&&)      = delete;

    [[nodiscard]] std::vector<int32_t> value() const {
        return getValueProvider().getInt32ArrayValue();
    }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type int64.
 *
 */
class DataPointInt64 : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = int64_t;

    ~DataPointInt64() override = default;

    DataPointInt64(const DataPointInt64&)            = delete;
    DataPointInt64(DataPointInt64&&)                 = delete;
    DataPointInt64& operator=(const DataPointInt64&) = delete;
    DataPointInt64& operator=(DataPointInt64&&)      = delete;

    [[nodiscard]] int64_t value() const { return getValueProvider().getInt64Value(); }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type int64 array.
 *
 */
class DataPointInt64Array : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = std::vector<int64_t>;

    ~DataPointInt64Array() override = default;

    DataPointInt64Array(const DataPointInt64Array&)            = delete;
    DataPointInt64Array(DataPointInt64Array&&)                 = delete;
    DataPointInt64Array& operator=(const DataPointInt64Array&) = delete;
    DataPointInt64Array& operator=(DataPointInt64Array&&)      = delete;

    [[nodiscard]] std::vector<int64_t> value() const {
        return getValueProvider().getInt64ArrayValue();
    }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type uint32.
 *
 */
class DataPointUint32 : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = uint32_t;

    ~DataPointUint32() override = default;

    DataPointUint32(const DataPointUint32&)            = delete;
    DataPointUint32(DataPointUint32&&)                 = delete;
    DataPointUint32& operator=(const DataPointUint32&) = delete;
    DataPointUint32& operator=(DataPointUint32&&)      = delete;

    [[nodiscard]] uint32_t value() const { return getValueProvider().getUint32Value(); }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type uint32 array.
 *
 */
class DataPointUint32Array : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = std::vector<uint32_t>;

    ~DataPointUint32Array() override = default;

    DataPointUint32Array(const DataPointUint32Array&)            = delete;
    DataPointUint32Array(DataPointUint32Array&&)                 = delete;
    DataPointUint32Array& operator=(const DataPointUint32Array&) = delete;
    DataPointUint32Array& operator=(DataPointUint32Array&&)      = delete;

    [[nodiscard]] std::vector<uint32_t> value() const {
        return getValueProvider().getUint32ArrayValue();
    }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type uint64.
 *
 */
class DataPointUint64 : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = uint64_t;

    ~DataPointUint64() override = default;

    DataPointUint64(const DataPointUint64&)            = delete;
    DataPointUint64(DataPointUint64&&)                 = delete;
    DataPointUint64& operator=(const DataPointUint64&) = delete;
    DataPointUint64& operator=(DataPointUint64&&)      = delete;

    [[nodiscard]] uint64_t value() const { return getValueProvider().getUint64Value(); }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type uint64 array.
 *
 */
class DataPointUint64Array : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = std::vector<uint64_t>;

    ~DataPointUint64Array() override = default;

    DataPointUint64Array(const DataPointUint64Array&)            = delete;
    DataPointUint64Array(DataPointUint64Array&&)                 = delete;
    DataPointUint64Array& operator=(const DataPointUint64Array&) = delete;
    DataPointUint64Array& operator=(DataPointUint64Array&&)      = delete;

    [[nodiscard]] std::vector<uint64_t> value() const {
        return getValueProvider().getUint64ArrayValue();
    }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type float.
 *
 */
class DataPointFloat : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = float;

    ~DataPointFloat() override = default;

    DataPointFloat(const DataPointFloat&)            = delete;
    DataPointFloat(DataPointFloat&&)                 = delete;
    DataPointFloat& operator=(const DataPointFloat&) = delete;
    DataPointFloat& operator=(DataPointFloat&&)      = delete;

    [[nodiscard]] float value() const { return getValueProvider().getFloatValue(); }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type float array.
 *
 */
class DataPointFloatArray : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = std::vector<float>;

    ~DataPointFloatArray() override = default;

    DataPointFloatArray(const DataPointFloatArray&)            = delete;
    DataPointFloatArray(DataPointFloatArray&&)                 = delete;
    DataPointFloatArray& operator=(const DataPointFloatArray&) = delete;
    DataPointFloatArray& operator=(DataPointFloatArray&&)      = delete;

    [[nodiscard]] std::vector<float> value() const {
        return getValueProvider().getFloatArrayValue();
    }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type double.
 *
 */
class DataPointDouble : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = double;

    ~DataPointDouble() override = default;

    DataPointDouble(const DataPointDouble&)            = delete;
    DataPointDouble(DataPointDouble&&)                 = delete;
    DataPointDouble& operator=(const DataPointDouble&) = delete;
    DataPointDouble& operator=(DataPointDouble&&)      = delete;

    [[nodiscard]] double value() const { return getValueProvider().getDoubleValue(); }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type double array.
 *
 */
class DataPointDoubleArray : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = std::vector<double>;

    ~DataPointDoubleArray() override = default;

    DataPointDoubleArray(const DataPointDoubleArray&)            = delete;
    DataPointDoubleArray(DataPointDoubleArray&&)                 = delete;
    DataPointDoubleArray& operator=(const DataPointDoubleArray&) = delete;
    DataPointDoubleArray& operator=(DataPointDoubleArray&&)      = delete;

    [[nodiscard]] std::vector<double> value() const {
        return getValueProvider().getDoubleArrayValue();
    }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type string.
 *
 */
class DataPointString : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = std::string;

    ~DataPointString() override = default;

    DataPointString(const DataPointString&)            = delete;
    DataPointString(DataPointString&&)                 = delete;
    DataPointString& operator=(const DataPointString&) = delete;
    DataPointString& operator=(DataPointString&&)      = delete;

    [[nodiscard]] std::string value() const { return getValueProvider().getStringValue(); }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point with the type string array.
 *
 */
class DataPointStringArray : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = std::vector<std::string>;

    ~DataPointStringArray() override = default;

    DataPointStringArray(const DataPointStringArray&)            = delete;
    DataPointStringArray(DataPointStringArray&&)                 = delete;
    DataPointStringArray& operator=(const DataPointStringArray&) = delete;
    DataPointStringArray& operator=(DataPointStringArray&&)      = delete;

    [[nodiscard]] std::vector<std::string> value() const {
        return getValueProvider().getStringArrayValue();
    }

    [[nodiscard]] std::string toString() const override;
};

/**
 * @brief A data point which caused a failure.
 *
 */
class DataPointFailure : public DataPoint {
public:
    DataPointFailure(std::string name, std::string failureReason);

    ~DataPointFailure() override = default;

    DataPointFailure(const DataPointFailure&)            = delete;
    DataPointFailure(DataPointFailure&&)                 = delete;
    DataPointFailure& operator=(const DataPointFailure&) = delete;
    DataPointFailure& operator=(DataPointFailure&&)      = delete;

    [[nodiscard]] bool isValid() const override { return false; }

    DataPointFailure& asFailure() override { return *this; }

    [[nodiscard]] const std::string& getReason() const { return m_failureReason; }

    [[nodiscard]] std::string toString() const override;

private:
    std::string m_failureReason;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_DATAPOINT_H
