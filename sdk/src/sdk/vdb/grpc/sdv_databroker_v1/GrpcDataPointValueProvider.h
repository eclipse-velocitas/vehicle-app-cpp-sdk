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

#include "IDataPointValueProvider.h"
#include "sdv/databroker/v1/types.grpc.pb.h"

namespace velocitas::sdv_databroker_v1 {

/**
 * @brief Value provider implementation for gRPC provided DataPoints
 *
 */
class GrpcDataPointValueProvider : public IDataPointValueProvider {
public:
    explicit GrpcDataPointValueProvider(sdv::databroker::v1::Datapoint datapoint);

    DataPointValue::Failure  getFailure() const override;
    bool                     getBoolValue() const override;
    std::vector<bool>        getBoolArrayValue() const override;
    float                    getFloatValue() const override;
    std::vector<float>       getFloatArrayValue() const override;
    double                   getDoubleValue() const override;
    std::vector<double>      getDoubleArrayValue() const override;
    int8_t                   getInt8Value() const override;
    std::vector<int8_t>      getInt8ArrayValue() const override;
    int16_t                  getInt16Value() const override;
    std::vector<int16_t>     getInt16ArrayValue() const override;
    int32_t                  getInt32Value() const override;
    std::vector<int32_t>     getInt32ArrayValue() const override;
    int64_t                  getInt64Value() const override;
    std::vector<int64_t>     getInt64ArrayValue() const override;
    uint8_t                  getUint8Value() const override;
    std::vector<uint8_t>     getUint8ArrayValue() const override;
    uint16_t                 getUint16Value() const override;
    std::vector<uint16_t>    getUint16ArrayValue() const override;
    uint32_t                 getUint32Value() const override;
    std::vector<uint32_t>    getUint32ArrayValue() const override;
    uint64_t                 getUint64Value() const override;
    std::vector<uint64_t>    getUint64ArrayValue() const override;
    std::string              getStringValue() const override;
    std::vector<std::string> getStringArrayValue() const override;
    Timestamp                getTimestamp() const;

protected:
    const sdv::databroker::v1::Datapoint& getDataPoint() const;

private:
    sdv::databroker::v1::Datapoint m_datapoint;
};

} // namespace velocitas::sdv_databroker_v1
