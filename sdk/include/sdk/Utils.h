/**
 * Copyright (c) 2022-2024 Contributors to the Eclipse Foundation
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
    /**
     * @brief Return the passed string converted to lowercase
     *
     * @param str string to be converted
     * @return std::string having all uppercase letters contained in the passed string converted to
     * lowercase
     */
    static std::string toLower(const std::string& str);

    /**
     * @brief Return the passed string converted to uppercase
     *
     * @param str string to be converted
     * @return std::string having all lowercase letters contained in the passed string converted to
     * uppercase
     */
    static std::string toUpper(const std::string& str);

    /**
     * @brief Concatenate the strings of the passed vector by adding the passed separator between
     * each two of the array elements.
     *
     * Examples:
     *
     * stringVector             | separator  | result
     * -------------------------|------------|--------------------
     * [] (empty vector)        | don't care | ""
     * ["hello"]                | don't care | "hello"
     * ["hello", "world", "eh"] | ", "       | "hello, world, eh"
     * ["single", "ton"]        | ""         | "singleton"
     *
     * @param stringVector vector of strings to be concatenated
     * @param separator string to be put between each two vector elements
     * @return std::string containing the joined contents
     */
    static std::string join(const std::vector<std::string>& stringVector,
                            const std::string&              separator);

private:
    StringUtils() = delete;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_UTILS_H
