/**
 * Copyright (c) 2023-2025 Contributors to the Eclipse Foundation
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

#ifndef VEHICLE_APP_SDK_TESTUSINGENVVARS_H
#define VEHICLE_APP_SDK_TESTUSINGENVVARS_H

#include <string>
#include <unordered_map>
#include <unordered_set>

#include <gtest/gtest.h>

namespace velocitas {

/**
 * @brief Base class for test cases where you need to set or unset environment variables
 *
 * It provides a setEnvVar and a unsetEnvVar function to be used for this by your test case.
 * After the test case is finished the original state of the changed variables is restored. *
 */
class TestUsingEnvVars : public ::testing::Test {
public:
    ~TestUsingEnvVars() = default;

protected:
    TestUsingEnvVars() = default;

    /**
     * @brief Restores the state of the set and unset variables as it was before executing the test
     * case
     */
    void TearDown() override;

    /**
     * @brief Set the an environment variable to the specified value
     *
     * @param varName  Name of the variable to be set
     * @param content  Value to be set
     */
    void setEnvVar(const std::string& varName, const std::string& content);

    /**
     * @brief Unset a possibly existing environment variable
     *
     * @param varName  Name of the variable to be unset
     */
    void unsetEnvVar(const std::string& varName);

private:
    void preserveEnvVarState(const std::string& varName);

    std::unordered_map<std::string, std::string> m_envVarsToRestore;
    std::unordered_set<std::string>              m_envVarsToUnset;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_TESTUSINGENVVARS_H
