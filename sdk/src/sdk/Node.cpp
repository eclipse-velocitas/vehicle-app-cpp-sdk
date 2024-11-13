/**
 * Copyright (c) 2022-2024 Contributors to the Eclipse Foundation
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

#include <stdexcept>
#include <utility>
#include <vector>

namespace velocitas {

static const char PATH_SEPARATOR = '.';

Node::Node(std::string name, Node* parent)
    : m_name(std::move(name))
    , m_parent(parent) {
    if (m_parent != nullptr) {
        m_parent->registerChild(*this);
    }
}

void Node::registerChild(const Node& childNode) {
    if (!m_children) {
        m_children = std::make_unique<NodeMap>();
    } else if (m_children->count(childNode.getName()) > 0) {
        throw std::runtime_error("Registering child with duplicate name!");
    }
    (*m_children)[childNode.getName()] = &childNode;
}

const DataPoint* Node::getDataPoint(const std::string& path) const {
    if (!m_children) {
        return nullptr;
    }

    auto separatorPos = path.find_first_of(PATH_SEPARATOR);
    auto childName    = path.substr(0, separatorPos);
    auto child        = m_children->find(childName);
    if (child == m_children->end()) {
        return nullptr;
    }

    auto subpathStart = (separatorPos != std::string::npos) ? separatorPos + 1 : path.length();
    auto subpath      = path.substr(subpathStart);
    return child->second->getDataPoint(subpath);
}

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

    return StringUtils::join(path, std::string(1, PATH_SEPARATOR));
}

} // namespace velocitas
