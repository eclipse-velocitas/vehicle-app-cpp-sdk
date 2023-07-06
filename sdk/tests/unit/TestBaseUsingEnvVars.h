/**
 * Copyright (c) 2023 Robert Bosch GmbH
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

class TestUsingEnvVars : public ::testing::Test {
public:
    ~TestUsingEnvVars() = default;

protected:
    TestUsingEnvVars() = default;

    // Tears down the test fixture.
    virtual void TearDown() override;

    void setEnvVar(const std::string& varName, const std::string& content);
    void unsetEnvVar(const std::string& varName);

private:
    void checkToStoreVariableState(const std::string& varName);

    std::unordered_map<std::string, std::string> m_envVarsToRestore;
    std::unordered_set<std::string>              m_envVarsToUnset;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_TESTUSINGENVVARS_H
