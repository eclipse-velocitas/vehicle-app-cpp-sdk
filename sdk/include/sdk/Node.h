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

#ifndef VEHICLE_APP_SDK_NODE_H
#define VEHICLE_APP_SDK_NODE_H

#include <string>

namespace velocitas {

/**
 * @brief A tree node.
 *
 */
class Node {
public:
    enum class Type {
        BRANCH,
        UNKNOWN_LEAF_TYPE, // Added for backward compatibility with older model generators
        ATTRIBUTE,
        SENSOR,
        ACTUATOR,
    };

    /**
     * @brief Construct a new Node object
     *
     * @param name    Name of the node.
     * @param parent  Parent of the node.
     */
    explicit Node(std::string name, Node* parent = nullptr);

    virtual ~Node() = default;

    /**
     * @brief Get the parent node of this Node
     *
     * @return const Node* Pointer to the parent or nullptr if this node represents the root of the
     * model
     */
    [[nodiscard]] const Node* getParent() const;

    /**
     * @brief Get the name of this node
     *
     * @return const std::string& being the name (not the path) of this node
     */
    [[nodiscard]] const std::string& getName() const;

    /**
     * @brief Return the fully qualified path of the node down from the root of the tree.
     *
     * @return std::string Fully qualified path of the node within the tree.
     */
    [[nodiscard]] std::string getPath() const;

    /**
     * @brief Get the type of the node
     * Possibly, needs being overridden by sub-classes
     *
     * @return Type::BRANCH as the Node base class always represents a branch of the model
     */
    [[nodiscard]] virtual Type getType() const { return Type::BRANCH; }

    Node(const Node&)            = delete;
    Node(Node&&)                 = delete;
    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&)      = delete;

private:
    // TODO: Use std::weak_ptr ?
    Node* const       m_parent;
    const std::string m_name;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_NODE_H
