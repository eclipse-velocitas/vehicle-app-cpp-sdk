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

#include "sdk/ThreadPool.h"

namespace velocitas {

ThreadPool::ThreadPool() {
    constexpr size_t numThreads{2};

    for (size_t i = 0; i < numThreads; ++i) {
        auto workerThread = std::make_unique<std::thread>([this]() { threadLoop(); });
        m_workerThreads.push_back(std::move(workerThread));
    }
}

ThreadPool::~ThreadPool() {
    m_isRunning = false;
    m_cv.notify_all();

    for (auto& thread : m_workerThreads) {
        thread->join();
    }
}

std::shared_ptr<ThreadPool> ThreadPool::getInstance() {
    static std::shared_ptr<ThreadPool> instance{nullptr};

    if (!instance) {
        instance = std::make_shared<ThreadPool>();
    }

    return instance;
}

void ThreadPool::execute(JobPtr_t job) {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    m_jobs.emplace(std::move(job));
    m_cv.notify_all();
}

void ThreadPool::threadLoop() {
    std::mutex waitMutex;
    while (m_isRunning) {
        JobPtr_t job;
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            if (!m_jobs.empty()) {
                job = m_jobs.front();
                m_jobs.pop();
            }
        }

        if (job) {
            job->execute(job, *this);
        } else {
            std::unique_lock<std::mutex> lock(waitMutex);
            m_cv.wait(lock);
        }
    }
}

} // namespace velocitas
