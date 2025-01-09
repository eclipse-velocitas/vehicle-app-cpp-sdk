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
#include "sdk/Utils.h"

#include <vector>

namespace velocitas {

Node::Node(std::string name, Node* parent)
    : m_name(std::move(name))
    , m_parent(parent) {}

const Node* Node::getParent() const { return m_parent; }

const std::string& Node::getName() const { return m_name; }

std::string Node::getPath() const {
    std::vector<std::string> path;
    path.push_back(m_name);
    auto* node = m_parent;
    while (node != nullptr) {
        path.insert(path.begin(), node->m_name);
        node = node->m_parent;
    }

    return StringUtils::join(path, ".");
}

} // namespace velocitas
