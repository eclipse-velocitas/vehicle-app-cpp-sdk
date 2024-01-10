/**
 * Copyright (c) 2022-2024 Contributors to the Eclipse Foundation
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
#include "sdk/Logger.h"

namespace velocitas {

ThreadPool::ThreadPool(size_t numWorkerThreads)
    : m_workerThreads{numWorkerThreads} {
    for (size_t i = 0; i < numWorkerThreads; ++i) {
        m_workerThreads[i] = std::thread([this]() { threadLoop(); });
    }
}

ThreadPool::ThreadPool()
    : ThreadPool(2) {}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard lock{m_queueMutex};
        m_isRunning = false;
        // empty the job queue (std::queue does not offer a clear function)
        std::queue<JobPtr_t>().swap(m_jobs);
    }
    m_cv.notify_all();

    for (auto& thread : m_workerThreads) {
        thread.join();
    }
}

std::shared_ptr<ThreadPool> ThreadPool::getInstance() {
    static std::shared_ptr<ThreadPool> instance{std::make_shared<ThreadPool>()};
    return instance;
}

size_t ThreadPool::getNumWorkerThreads() const { return m_workerThreads.size(); }

void ThreadPool::enqueue(JobPtr_t job) {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    m_jobs.emplace(std::move(job));
    m_cv.notify_one();
}

void ThreadPool::threadLoop() {
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
            try {
                job->execute();
                if (job->shallRecur()) {
                    enqueue(job);
                }
            } catch (const std::exception& e) {
                logger().error("[ThreadPool] Uncaught exception in job execution: " +
                               std::string(e.what()));
            } catch (...) {
                logger().error(
                    std::string("[ThreadPool] Uncaught unknown exception in job execution"));
            }
        } else {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_cv.wait(lock, [this] { return !m_jobs.empty() || !m_isRunning; });
        }
    }
}

} // namespace velocitas
