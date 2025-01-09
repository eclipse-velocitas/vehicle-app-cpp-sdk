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
    /**
     * @brief Create a QueryBuilder selecting a single data point
     *
     * @param dataPoint   Reference to data point to be selected
     * @return A new instance of QueryBuilder
     */
    static QueryBuilder select(const DataPoint& dataPoint);

    /**
     * @brief Create a QueryBuilder selecting a multiple data points
     *
     * @param dataPoint   List of references to data points to be selected
     * @return A new instance of QueryBuilder
     */
    static QueryBuilder select(const std::vector<std::reference_wrapper<DataPoint>>& dataPoints);

    /**
     * @brief Adds a condition for a data point to be met for getting a notification
     *
     * @param dataPoint   Reference to the data point to add the condition for
     * @return A WhereClauseBuilder, where a condition can be added
     */
    template <typename TDataPoint>
    WhereClauseBuilder<typename TDataPoint::value_type> where(const TDataPoint& dataPoint) {
        return WhereClauseBuilder<typename TDataPoint::value_type>(this, dataPoint);
    }

    /**
     * @brief Returns the compiled query to be passed to the data broker.
     * Need to be called finally, after the desired query expression is completed.
     *
     * @return The compiled query
     */
    [[nodiscard]] std::string build() const;

private:
    QueryBuilder() = default;

    std::vector<std::string> m_queryContext;

    template <typename T> friend class WhereClauseBuilder;
};

/**
 * @brief Builder for where subclauses (i.e. a condition) within the VDB query.
 *
 */
template <typename T> class WhereClauseBuilder final {
public:
    /**
     * @brief Adds a greater than condition for the chosen data point
     *
     * @param value   The value of the data point must be greater than this
     * @return This instance for method chaining.
     */
    WhereClauseBuilder& gt(T value) {
        m_parent->m_queryContext.emplace_back(">");
        m_parent->m_queryContext.emplace_back(std::to_string(value));
        return *this;
    }

    /**
     * @brief Adds a lesser than condition for the chosen data point
     *
     * @param value   The value of the data point must be smaller than this
     * @return This instance for method chaining.
     */
    WhereClauseBuilder& lt(T value) {
        m_parent->m_queryContext.emplace_back("<");
        m_parent->m_queryContext.emplace_back(std::to_string(value));
        return *this;
    }

    /**
     * @brief Adds a equals to condition for the chosen data point
     *
     * @param value   The value of the data point must be equal to this
     * @return This instance for method chaining.
     */
    WhereClauseBuilder& eq(T value) {
        m_parent->m_queryContext.emplace_back("=");
        m_parent->m_queryContext.emplace_back(std::to_string(value));
        return *this;
    }

    /**
     * @brief Returns the compiled query to be passed to the data broker.
     * Need to be called finally, after the desired query expression is completed.
     *
     * @return The compiled query
     */
    [[nodiscard]] std::string build() const { return m_parent->build(); }

private:
    WhereClauseBuilder(QueryBuilder* parent, const DataPoint& dataPoint)
        : m_parent{parent} {
        m_parent->m_queryContext.emplace_back("WHERE");
        m_parent->m_queryContext.emplace_back(dataPoint.getPath());
    }

    QueryBuilder* m_parent{nullptr};

    friend class QueryBuilder;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_QUERYBUILDER_H
