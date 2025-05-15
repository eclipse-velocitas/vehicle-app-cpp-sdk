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

#include "sdk/AsyncResult.h"

#include <gtest/gtest.h>

using namespace velocitas;

TEST(Test_ScopedBoolInverter, create_boolIsFalse_boolIsTrue) {
    bool               testBool = false;
    ScopedBoolInverter cut{testBool};
    EXPECT_TRUE(testBool);
}

TEST(Test_ScopedBoolInverter, create_boolIsTrue_boolIsFalse) {
    bool               testBool = true;
    ScopedBoolInverter cut{testBool};
    EXPECT_FALSE(testBool);
}

TEST(Test_ScopedBoolInverter, destroy_boolSetToFalseAfterConstruction_boolIsTrue) {
    bool testBool = false;
    {
        ScopedBoolInverter cut{testBool};
        testBool = false;
    }
    EXPECT_TRUE(testBool);
}

TEST(Test_ScopedBoolInverter, destroy_boolSetToTrueAfterConstruction_boolIsFalse) {
    bool testBool = false;
    {
        ScopedBoolInverter cut{testBool};
        testBool = true;
    }
    EXPECT_FALSE(testBool);
}
