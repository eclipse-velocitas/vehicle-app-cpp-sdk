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

#include "sdk/ThreadPool.h"
#include "sdk/Logger.h"

#include <cassert>

namespace velocitas {

ThreadPool::ThreadPool(size_t numWorkerThreads)
    : m_jobs(&lowerJobPriority)
    , m_workerThreads{numWorkerThreads} {
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
        // empty the job queue (std::priority_queue does not offer a clear function)
        QueueType().swap(m_jobs);
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
    if (job) {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_jobs.push(std::move(job));
        m_cv.notify_one();
    } else {
        logger().error("[ThreadPool::enqueue] Ignoring nullptr Job!");
        assert(job);
    }
}

JobPtr_t ThreadPool::getNextExecutableJob() {
    JobPtr_t                    job;
    std::lock_guard<std::mutex> lock(m_queueMutex);
    if (!m_jobs.empty() && m_jobs.top()->isDue()) {
        job = m_jobs.top();
        m_jobs.pop();
    }
    return job;
}

void ThreadPool::waitForPotentiallyExecutableJob() const {
    std::unique_lock<std::mutex> lock(m_queueMutex);
    if (m_jobs.empty()) {
        m_cv.wait(lock, [this] { return !m_jobs.empty() || !m_isRunning; });
    } else {
        auto timepointToExecuteNextJob = m_jobs.top()->getTimepointToExecute();
        m_cv.wait_until(lock, timepointToExecuteNextJob);
    }
}

namespace {
void executeJob(JobPtr_t job) {
    try {
        job->execute();
    } catch (const std::exception& e) {
        logger().error("[ThreadPool] Uncaught exception during job execution: " +
                       std::string(e.what()));
    } catch (...) {
        logger().error(std::string("[ThreadPool] Uncaught unknown exception during job execution"));
    }
}
} // namespace

void ThreadPool::threadLoop() {
    while (m_isRunning) {
        JobPtr_t job = getNextExecutableJob();
        if (job) {
            executeJob(job);
            if (job->shallRecur()) {
                enqueue(job);
            }
        } else {
            waitForPotentiallyExecutableJob();
        }
    }
}

} // namespace velocitas
