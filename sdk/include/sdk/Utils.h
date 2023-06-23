/**
 * Copyright (c) 2022-2023 Robert Bosch GmbH
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

#ifndef VEHICLE_APP_SDK_UTILS_H
#define VEHICLE_APP_SDK_UTILS_H

#include <string>
#include <vector>

namespace velocitas {

/**
 * @brief Get the value of the specified environment variable
 *
 * @param varName  Name of the environment variable
 * @param defaultValue  Default if variable is not set
 * @return std::string containing the value of the variable or the default value
 */
std::string getEnvVar(const std::string& varName, const std::string& defaultValue = "");

/**
 * @brief Provides utility methods for handling strings.
 *
 */
class StringUtils final {
public:
    static std::string toLower(const std::string& str);
    static std::string toUpper(const std::string& str);

    static std::string join(const std::vector<std::string>& stringVector,
                            const std::string&              separator);

private:
    StringUtils() = default;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_UTILS_H
