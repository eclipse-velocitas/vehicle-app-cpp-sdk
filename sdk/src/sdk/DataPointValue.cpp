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

#include "sdk/DataPointValue.h"

#include "sdk/Logger.h"

#include <fmt/core.h>

#include <cassert>
#include <string>

namespace velocitas {

std::string toString(const DataPointValue::Failure failure) {
    switch (failure) {
    case DataPointValue::Failure::NONE:
        return "DataPointValue::Failure::NONE";
    case DataPointValue::Failure::INVALID_VALUE:
        return "DataPointValue::Failure::INVALID_VALUE";
    case DataPointValue::Failure::NOT_AVAILABLE:
        return "DataPointValue::Failure::NOT_AVAILABLE";
    case DataPointValue::Failure::UNKNOWN_DATAPOINT:
        return "DataPointValue::Failure::UNKNOWN_DATAPOINT";
    case DataPointValue::Failure::ACCESS_DENIED:
        return "DataPointValue::Failure::ACCESS_DENIED";
    case DataPointValue::Failure::INTERNAL_ERROR:
        return "DataPointValue::Failure::INTERNAL_ERROR";
    default:
        logger().error("velocitas::toString(DataPointValue::Failure): Unknown "
                       "DataPointValue::Failure enum value {}",
                       static_cast<unsigned int>(failure));
        assert(false);
        return fmt::format("DataPointValue::Failure::<unknown-{}>",
                           static_cast<unsigned int>(failure));
    }
}

} // namespace velocitas
