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

#ifndef VEHICLE_APP_SDK_NODE_H
#define VEHICLE_APP_SDK_NODE_H

#include <memory>
#include <string>

namespace velocitas {

/**
 * @brief A tree node.
 *
 */
class Node {
public:
    /**
     * @brief Construct a new Node object
     *
     * @param name    Name of the node.
     * @param parent  Parent of the node.
     */
    explicit Node(std::string name, Node* parent = nullptr);

    virtual ~Node(){};

    [[nodiscard]] const Node* getParent() const;

    [[nodiscard]] const std::string& getName() const;

    /**
     * @brief Return the fully qualified path of the node down from the root of the tree.
     *
     * @return std::string Fully qualified path of the node within the tree.
     */
    [[nodiscard]] std::string getPath() const;

    Node(const Node&)            = delete;
    Node(Node&&)                 = delete;
    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&)      = delete;

private:
    // TODO: Use std::weak_ptr ?
    Node*       m_parent;
    std::string m_name;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_NODE_H
