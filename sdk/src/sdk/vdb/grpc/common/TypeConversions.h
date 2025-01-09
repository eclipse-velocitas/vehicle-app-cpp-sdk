/**
 * Copyright (c) 2024-2025 Contributors to the Eclipse Foundation
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

#ifndef VEHICLE_APP_SDK_VDB_GRPC_COMMON_TYPECONVERSIONS_H
#define VEHICLE_APP_SDK_VDB_GRPC_COMMON_TYPECONVERSIONS_H

namespace google::protobuf {
class Timestamp;
}

namespace velocitas {

class Timestamp;

Timestamp convertFromGrpcTimestamp(const google::protobuf::Timestamp& grpcTimestamp) noexcept;

} // namespace velocitas

#endif // VEHICLE_APP_SDK_VDB_GRPC_COMMON_TYPECONVERSIONS_H
