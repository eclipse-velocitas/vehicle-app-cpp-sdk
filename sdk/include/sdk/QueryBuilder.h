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

#ifndef VEHICLE_APP_SDK_QUERYBUILDER_H
#define VEHICLE_APP_SDK_QUERYBUILDER_H

#include "sdk/Node.h"

#include <string>
#include <vector>

namespace velocitas {

/**
 * @brief Builder for where subclauses within the VDB query.
 *
 */
class WhereClauseBuilder final {
public:
    WhereClauseBuilder() = default;

    WhereClauseBuilder& dataPoint(const Node& node);
    WhereClauseBuilder& matchesCriteria(const std::string& criteria);

    std::string build();
};

/**
 * @brief Builder for VDB queries.
 *
 */
class QueryBuilder final {
public:
    static QueryBuilder select(const Node& node);

    QueryBuilder& join(std::vector<const Node*> args);
    QueryBuilder& where(const std::string& condition);

    std::string build();

private:
    QueryBuilder() = default;

    std::vector<std::string> m_queryContext;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_QUERYBUILDER_H
