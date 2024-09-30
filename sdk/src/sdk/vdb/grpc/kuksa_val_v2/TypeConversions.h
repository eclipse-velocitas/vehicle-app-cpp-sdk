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

#ifndef VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_TYPECONVERSIONS_H
#define VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_TYPECONVERSIONS_H

#include "kuksa/val/v2/val.grpc.pb.h"

#include "sdk/DataPointValue.h"

#include <memory>
#include <string>

namespace velocitas::kuksa_val_v2 {

kuksa::val::v2::Value convertToGrpcValue(const DataPointValue& dataPoint);

// DataPointValue::Failure
// convertFromGrpcValueFailure(const kuksa::val::v2::ValueFailure& grpcValueFailure);

std::shared_ptr<DataPointValue> convertFromGrpcValue(const std::string&           path,
                                                     const kuksa::val::v2::Value& value,
                                                     const Timestamp&             timestamp);
std::shared_ptr<DataPointValue>
convertFromGrpcDataPoint(const std::string& path, const kuksa::val::v2::Datapoint& grpcDataPoint);

void parseQueryIntoRequest(kuksa::val::v2::SubscribeRequest& request, const std::string& query);

} // namespace velocitas::kuksa_val_v2

#endif // VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_TYPECONVERSIONS_H
