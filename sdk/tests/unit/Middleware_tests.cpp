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

#include "TestBaseUsingEnvVars.h"
#include <gtest/gtest.h>

#define private public
#include "sdk/middleware/Middleware.h"
#include "sdk/middleware/NativeMiddleware.h"

using namespace velocitas;

class Test_Middleware : public TestUsingEnvVars {};

TEST_F(Test_Middleware, getInstance_envVarGloballySetToNative_typeIdIsNative) {
    const Middleware& middleware = Middleware::getInstance();
    EXPECT_EQ(NativeMiddleware::TYPE_ID, middleware.getTypeId());
}

TEST_F(Test_Middleware, instantiate_envVarNotSet_typeIdDefaultsToNative) {
    unsetEnvVar(Middleware::TYPE_DEFINING_ENV_VAR_NAME);
    auto middleware = Middleware::instantiate();
    EXPECT_EQ(NativeMiddleware::TYPE_ID, middleware->getTypeId());
}

TEST_F(Test_Middleware, instantiate_envVarSetToNative_typeIdIsNative) {
    setEnvVar(Middleware::TYPE_DEFINING_ENV_VAR_NAME, NativeMiddleware::TYPE_ID);
    EXPECT_EQ(NativeMiddleware::TYPE_ID, Middleware::instantiate()->getTypeId());
}

TEST_F(Test_Middleware, instantiate_envVarSetToUndefined_throwRuntimeError) {
    setEnvVar(Middleware::TYPE_DEFINING_ENV_VAR_NAME, "something undefined");
    EXPECT_THROW(Middleware::instantiate(), std::runtime_error);
}
