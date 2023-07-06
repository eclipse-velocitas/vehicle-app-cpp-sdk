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

#include "TestBaseUsingEnvVars.h"
#include <gtest/gtest.h>

#include <cstdlib>

#define private public
#include "sdk/middleware/DaprMiddleware.h"
#include "sdk/middleware/Middleware.h"
#include "sdk/middleware/NativeMiddleware.h"

using namespace velocitas;

class Test_Middleware : public TestUsingEnvVars {};

TEST_F(Test_Middleware, getTypeDefiningEnvVarName__noneEmptyString) {
    EXPECT_FALSE(Middleware::getTypeDefiningEnvVarName().empty());
}

TEST_F(Test_Middleware, getInstantance_envVarGloballySetToNative_TypeIdIsNative) {
    const Middleware& middleware = Middleware::getInstance();
    EXPECT_EQ(NativeMiddleware::TYPE_ID, middleware.getTypeId());
}

TEST_F(Test_Middleware, instantiate_envVarNotSet_typeIdIsDapr) {
    ::unsetenv(Middleware::getTypeDefiningEnvVarName().c_str());
    auto middleware = Middleware::instantiate();
    EXPECT_EQ(DaprMiddleware::TYPE_ID, middleware->getTypeId());
}

TEST_F(Test_Middleware, instantiate_envVarSetToDapr_typeIdIsDapr) {
    ::setenv(Middleware::getTypeDefiningEnvVarName().c_str(), DaprMiddleware::TYPE_ID,
             /*overwrite=*/true);
    EXPECT_EQ(DaprMiddleware::TYPE_ID, Middleware::instantiate()->getTypeId());
}

TEST_F(Test_Middleware, instantiate_envVarSetToNative_TypeIdIsNative) {
    ::setenv(Middleware::getTypeDefiningEnvVarName().c_str(), NativeMiddleware::TYPE_ID,
             /*overwrite=*/true);
    EXPECT_EQ(NativeMiddleware::TYPE_ID, Middleware::instantiate()->getTypeId());
}

TEST_F(Test_Middleware, instantiate_envVarSetToUndefined_throwRuntimeError) {
    ::setenv(Middleware::getTypeDefiningEnvVarName().c_str(), "something undefined",
             /*overwrite=*/true);
    EXPECT_THROW(Middleware::instantiate(), std::runtime_error);
}
