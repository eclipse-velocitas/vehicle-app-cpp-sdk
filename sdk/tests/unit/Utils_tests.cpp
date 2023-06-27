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

#include "sdk/Utils.h"

#include <gtest/gtest.h>

using namespace velocitas;

TEST(Utils, getEnvVar_varNotSet_defaultValue) {
    ::unsetenv("MY_TEST_ENV_VAR");
    std::string varContent = getEnvVar("MY_TEST_ENV_VAR", "default content");
    EXPECT_EQ("default content", varContent);
}

TEST(Utils, getEnvVar_emptyVar_emptyString) {
    ::setenv("MY_TEST_ENV_VAR", "", /*overwrite=*/true);
    std::string varContent = getEnvVar("MY_TEST_ENV_VAR", "default content");
    EXPECT_EQ("", varContent);
}

TEST(Utils, getEnvVar_nonEmptyVar_varContent) {
    ::setenv("MY_TEST_ENV_VAR", "some content", /*overwrite=*/true);
    std::string varContent = getEnvVar("MY_TEST_ENV_VAR", "default content");
    EXPECT_EQ("some content", varContent);
}

TEST(StringUtils, toLower_emptyString_emptyString) {
    EXPECT_TRUE(StringUtils::toLower("").empty());
}

TEST(StringUtils, toUpper_emptyString_emptyString) {
    EXPECT_TRUE(StringUtils::toUpper("").empty());
}

static const std::string
    MixedAscii(" !\"#$%&'()*+,-./"
               "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");

TEST(StringUtils, toLower_asciiString_allUpperAreLower) {
    EXPECT_EQ(" !\"#$%&'()*+,-./"
              "0123456789:;<=>?@abcdefghijklmnopqrstuvwxyz[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~",
              StringUtils::toLower(MixedAscii));
}

TEST(StringUtils, toUpper_asciiString_allLowerAreUpper) {
    EXPECT_EQ(" !\"#$%&'()*+,-./"
              "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`ABCDEFGHIJKLMNOPQRSTUVWXYZ{|}~",
              StringUtils::toUpper(MixedAscii));
}

TEST(StringUtils, join_emptyVector_emptyString) {
    std::vector<std::string> v;
    auto                     result = StringUtils::join(v, ",");
    EXPECT_EQ(result, "");
}

TEST(StringUtils, join_vectorWithOneElement_elementJoinedWithNoSeparator) {
    std::vector<std::string> v      = {"foo"};
    auto                     result = StringUtils::join(v, ",");
    EXPECT_EQ(result, "foo");
}

TEST(StringUtils, join_vectorWithMultipleElements_elementsJoinedWithSeparator) {
    std::vector<std::string> v      = {"foo", "bar", "baz"};
    auto                     result = StringUtils::join(v, ",");
    EXPECT_EQ(result, "foo,bar,baz");
}
