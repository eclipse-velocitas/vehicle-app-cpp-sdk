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

#include "sdk/middleware/Middleware.h"
#include "sdk/middleware/NativeMiddleware.h"

#include <cstdlib>
#include <gtest/gtest.h>

using namespace velocitas;

class SetupMiddlewareSingleton : public ::testing::Environment {
public:
    void SetUp() override {
        ::setenv(Middleware::TYPE_DEFINING_ENV_VAR_NAME, NativeMiddleware::TYPE_ID,
                 /*overwrite=*/true);
        std::ignore = Middleware::getInstance();
    }
};

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    std::ignore = testing::AddGlobalTestEnvironment(new SetupMiddlewareSingleton);
    return RUN_ALL_TESTS();
}
