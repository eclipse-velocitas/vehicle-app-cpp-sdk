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

#include "sdk/Utils.h"

#include <gtest/gtest.h>

using namespace velocitas;

TEST(Utils, getEnvVar_varNotSet_defaultValue) {
    ::unsetenv("MY_TEST_ENV_VAR");
    std::string varContent = getEnvVar("MY_TEST_ENV_VAR", "default content");
    EXPECT_EQ("default content", varContent);
}

TEST(Utils, getEnvVar_emptyVar_emptyString) {
    ::setenv("MY_TEST_ENV_VAR", "", /*overwrite=*/1);
    std::string varContent = getEnvVar("MY_TEST_ENV_VAR", "default content");
    EXPECT_EQ("", varContent);
}

TEST(Utils, getEnvVar_nonEmptyVar_varContent) {
    ::setenv("MY_TEST_ENV_VAR", "some content", /*overwrite=*/1);
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

TEST(SimpleUrlParse, emptyString) {
    SimpleUrlParse cut("");
    EXPECT_EQ("", cut.getScheme());
    EXPECT_EQ("", cut.getNetLocation());
}

TEST(SimpleUrlParse, noScheme_noSlashes_hostname_noPort_noPath) {
    SimpleUrlParse cut("localhost");
    EXPECT_EQ("", cut.getScheme());
    EXPECT_EQ("localhost", cut.getNetLocation());
}

TEST(SimpleUrlParse, noScheme_noSlashes_ipAddress_noPort_noPath) {
    SimpleUrlParse cut("1.2.3.4");
    EXPECT_EQ("", cut.getScheme());
    EXPECT_EQ("1.2.3.4", cut.getNetLocation());
}

TEST(SimpleUrlParse, ctor_noScheme_slashes_hostname_port_noPath) {
    SimpleUrlParse cut("//localhost:42");
    EXPECT_EQ("", cut.getScheme());
    EXPECT_EQ("localhost:42", cut.getNetLocation());
}

TEST(SimpleUrlParse, ctor_noScheme_slashes_user_password_hostname_port_noPath) {
    SimpleUrlParse cut("//username:password@localhost:42");
    EXPECT_EQ("", cut.getScheme());
    EXPECT_EQ("username:password@localhost:42", cut.getNetLocation());
}

TEST(SimpleUrlParse, ctor_someScheme_slashes_user_password_hostname_port_emptyPath) {
    SimpleUrlParse cut("WhatEver://username:password@localhost:42/");
    EXPECT_EQ("whatever", cut.getScheme());
    EXPECT_EQ("username:password@localhost:42", cut.getNetLocation());
}

TEST(SimpleUrlParse, ctor_someScheme_slashes_ipAddress_noPort_pathAndQuery) {
    SimpleUrlParse cut("SomeScheme://1.2.3.4/somePath?query");
    EXPECT_EQ("somescheme", cut.getScheme());
    EXPECT_EQ("1.2.3.4", cut.getNetLocation());
}
TEST(SimpleUrlParse, ctor_unix_domain_socket) {
    SimpleUrlParse cut("unix:///some/path/to/socket");
    EXPECT_EQ("unix", cut.getScheme());
    EXPECT_EQ("unix:///some/path/to/socket", cut.getNetLocation());
}
