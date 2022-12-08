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

#include "sdk/DataPoint.h"

#include <string>
#include <vector>

namespace velocitas {

template <typename T> class WhereClauseBuilder;

/**
 * @brief Builder for VDB queries.
 *
 */
class QueryBuilder final {
public:
    static QueryBuilder select(const DataPoint& dataPoint);

    static QueryBuilder select(const std::vector<std::reference_wrapper<DataPoint>>& dataPoints);

    template <typename TDataPoint>
    WhereClauseBuilder<typename TDataPoint::value_type> where(const TDataPoint& dataPoint) {
        return WhereClauseBuilder<typename TDataPoint::value_type>(this, dataPoint);
    }

    [[nodiscard]] std::string build() const;

private:
    QueryBuilder() = default;

    std::vector<std::string> m_queryContext;

    template <typename T> friend class WhereClauseBuilder;
};

/**
 * @brief Builder for where subclauses within the VDB query.
 *
 */
template <typename T> class WhereClauseBuilder final {
public:
    WhereClauseBuilder(QueryBuilder* parent, const DataPoint& dataPoint)
        : m_parent{parent} {
        m_parent->m_queryContext.emplace_back("WHERE");
        m_parent->m_queryContext.emplace_back(dataPoint.getPath());
    }

    WhereClauseBuilder& gt(T value) {
        m_parent->m_queryContext.emplace_back(">");
        m_parent->m_queryContext.emplace_back(std::to_string(value));
        return *this;
    }

    WhereClauseBuilder& lt(T value) {
        m_parent->m_queryContext.emplace_back("<");
        m_parent->m_queryContext.emplace_back(std::to_string(value));
        return *this;
    }

    WhereClauseBuilder& eq(T value) {
        m_parent->m_queryContext.emplace_back("=");
        m_parent->m_queryContext.emplace_back(std::to_string(value));
        return *this;
    }

    [[nodiscard]] std::string build() const { return m_parent->build(); }

private:
    QueryBuilder* m_parent{nullptr};
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_QUERYBUILDER_H
