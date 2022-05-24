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

#include "sdk/Job.h"
#include "sdk/ThreadPool.h"

namespace velocitas {

Job::Job(std::function<void()> fun)
    : m_fun(std::move(fun)) {}

void Job::waitForTermination() { std::scoped_lock lock(m_terminationMutex); }

void Job::execute(JobPtr_t& /*thisJobPtr*/, ThreadPool& /*pool*/) {
    std::scoped_lock lock(m_terminationMutex);
    m_fun();
}

void RecurringJob::execute(JobPtr_t& thisJobPtr, ThreadPool& pool) {
    if (!m_isCancelled) {
        Job::execute(thisJobPtr, pool);
    }

    if (!m_isCancelled) {
        pool.execute(thisJobPtr);
    }
}

} // namespace velocitas
