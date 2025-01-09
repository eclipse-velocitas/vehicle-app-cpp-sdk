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

#ifndef VEHICLE_APP_SDK_MODEL_H
#define VEHICLE_APP_SDK_MODEL_H

#include "sdk/DataPointBatch.h"
#include "sdk/Node.h"
#include "sdk/middleware/Middleware.h"

#include <string>

namespace velocitas {

/**
 * @brief The Model class represents a branch of the model tree, including root.
 * Leafs are typcially one of the typed DataPoint* classes.
 * But also a Model class can be a leaf, if it does not contain data Points, just methods.
 */
class Model : public Node {
public:
    using Node::Node;

    [[nodiscard]] DataPointBatch setMany() const { return DataPointBatch{}; }
};

/**
 * @brief Base class for services within the model tree.
 *
 */
class Service : public Node {
public:
    using Node::Node;

    [[nodiscard]] std::string          getLocation() const;
    [[nodiscard]] Middleware::Metadata getMiddlewareMetadata() const;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_MODEL_H
