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

#include "sdk/Node.h"
#include <gtest/gtest.h>

using namespace velocitas;

TEST(Test_Node, constructor_withoutParent_isRootNoParent) {
    Node node{"foo"};

    EXPECT_EQ(node.getName(), "foo");
    EXPECT_EQ(node.getParent(), nullptr);
    EXPECT_EQ(node.getPath(), "foo");
}

TEST(Test_Node, constructor_withParent_isLeaf) {
    Node nodeRoot{"root"};
    Node node{"foo", &nodeRoot};

    EXPECT_EQ(node.getName(), "foo");
    EXPECT_EQ(node.getParent(), &nodeRoot);
    EXPECT_EQ(node.getPath(), "root.foo");
}
