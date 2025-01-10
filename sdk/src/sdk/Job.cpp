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

#include "sdk/Job.h"

namespace velocitas {

bool lowerJobPriority(const JobPtr_t& left, const JobPtr_t& right) {
    return left->getTimepointToExecute() > right->getTimepointToExecute();
}

Job::Job(std::function<void()> fun, std::chrono::milliseconds delay)
    : m_fun(std::move(fun)) {
    if (delay > std::chrono::milliseconds::zero()) {
        m_timepointToExecute = Clock::now() + delay;
    }
}

void Job::waitForTermination() const { std::lock_guard lock(m_terminationMutex); }

void Job::execute() {
    std::lock_guard lock(m_terminationMutex);
    m_fun();
}

void RecurringJob::execute() {
    if (!m_isCancelled) {
        Job::execute();
    }
}

} // namespace velocitas
