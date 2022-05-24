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

#include "sdk/QueryBuilder.h"
#include "sdk/Utils.h"

namespace velocitas {

QueryBuilder QueryBuilder::select(const Node& node) {
    QueryBuilder builder;
    builder.m_queryContext.emplace_back("SELECT");
    builder.m_queryContext.push_back(node.getPath());
    return builder;
}

QueryBuilder& QueryBuilder::join(std::vector<const Node*> args) {
    for (const auto* const arg : args) {
        m_queryContext.emplace_back(",");
        m_queryContext.push_back(arg->getPath());
    }

    return *this;
}

QueryBuilder& QueryBuilder::where(const std::string& condition) {
    m_queryContext.emplace_back("WHERE");
    m_queryContext.emplace_back(condition);

    return *this;
}

std::string QueryBuilder::build() { return StringUtils::join(m_queryContext, " "); }

} // namespace velocitas
