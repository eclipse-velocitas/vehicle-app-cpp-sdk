/**
 * Copyright (c) 2022-2023 Contributors to the Eclipse Foundation
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

#include "sdk/Utils.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <sstream>

namespace velocitas {

std::string getEnvVar(const std::string& varName, const std::string& defaultValue) {
    auto* const value = ::getenv(varName.c_str());
    if (value != nullptr) {
        return std::string(value);
    }
    return defaultValue;
}

std::string StringUtils::toLower(const std::string& str) {
    std::string result{str};
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string StringUtils::toUpper(const std::string& str) {
    std::string result{str};
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string StringUtils::join(const std::vector<std::string>& stringVector,
                              const std::string&              separator) {
    std::ostringstream oss;
    if (!stringVector.empty()) {
        oss << stringVector.front();
        std::for_each(stringVector.begin() + 1, stringVector.end(),
                      [&oss, &separator](const std::string& elem) { oss << separator << elem; });
    }
    return oss.str();
}

} // namespace velocitas
