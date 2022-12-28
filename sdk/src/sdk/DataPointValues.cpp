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

#include "sdk/DataPointValues.h"

#include "sdk/Logger.h"

#include <fmt/core.h>

#include <cassert>
#include <string>

namespace velocitas {

std::string toString(const DataPointFailure failure) {
    switch (failure) {
    case DataPointFailure::NONE:
        return "DataPointFailure::NONE";
    case DataPointFailure::INVALID_VALUE:
        return "DataPointFailure::INVALID_VALUE";
    case DataPointFailure::NOT_AVAILABLE:
        return "DataPointFailure::NOT_AVAILABLE";
    case DataPointFailure::UNKNOWN_DATAPOINT:
        return "DataPointFailure::UNKNOWN_DATAPOINT";
    case DataPointFailure::ACCESS_DENIED:
        return "DataPointFailure::ACCESS_DENIED";
    case DataPointFailure::INTERNAL_ERROR:
        return "DataPointFailure::INTERNAL_ERROR";
    default:
        logger().error(
            "velocitas::toString(DataPointFailure): Unknown DataPointFailure enum value {}",
            static_cast<unsigned int>(failure));
        assert(false);
        return fmt::format("DataPointFailure::<unknown-{}>", static_cast<unsigned int>(failure));
    }
}

} // namespace velocitas
