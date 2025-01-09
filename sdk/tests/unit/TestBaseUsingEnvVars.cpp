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

#include "TestBaseUsingEnvVars.h"

#include <cstdlib>
#include <string>

namespace velocitas {

void TestUsingEnvVars::preserveEnvVarState(const std::string& varName) {
    if (m_envVarsToRestore.count(varName) > 0 || m_envVarsToUnset.count(varName) > 0) {
        return;
    }

    const char* content = ::getenv(varName.c_str());
    if (content) {
        m_envVarsToRestore[varName] = *content;
    } else {
        m_envVarsToUnset.insert(varName);
    }
}

void TestUsingEnvVars::setEnvVar(const std::string& varName, const std::string& content) {
    preserveEnvVarState(varName);
    ::setenv(varName.c_str(), content.c_str(), /*replace=*/true);
}

void TestUsingEnvVars::unsetEnvVar(const std::string& varName) {
    preserveEnvVarState(varName);
    ::unsetenv(varName.c_str());
}

void TestUsingEnvVars::TearDown() {
    for (auto varToUnset : m_envVarsToUnset) {
        ::unsetenv(varToUnset.c_str());
    }
    for (auto varToRestore : m_envVarsToRestore) {
        ::setenv(varToRestore.first.c_str(), varToRestore.second.c_str(), /*replace=*/true);
    }
}

} // namespace velocitas
