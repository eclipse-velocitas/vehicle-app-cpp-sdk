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

#ifndef VEHICLE_APP_SDK_JOB_H
#define VEHICLE_APP_SDK_JOB_H

#include <functional>
#include <memory>
#include <mutex>

namespace velocitas {

class ThreadPool;
class IJob;
using JobPtr_t = std::shared_ptr<IJob>;

/**
 * @brief Interface for jobs which can be executed by a worker in the thread pool.
 *
 */
class IJob {
public:
    IJob()          = default;
    virtual ~IJob() = default;

    /**
     * @brief Execute the job.
     *
     * @param thisJobPtr    A smart pointer to this job. Allows sub classes to i.e. re-trigger the
     *                      very same job instance.
     * @param pool          The pool that is executing the job. Allows sub classes to i.e.
     *                      re-trigger the same job.
     */
    virtual void execute(JobPtr_t& thisJobPtr, ThreadPool& pool) = 0; // NOLINT

    IJob(const IJob&)            = delete;
    IJob(IJob&&)                 = delete;
    IJob& operator=(const IJob&) = delete;
    IJob& operator=(IJob&&)      = delete;
};

/**
 * @brief A nonrecurring job.
 *
 */
class Job : public IJob {
public:
    static JobPtr_t create(std::function<void()> fun) { return std::make_shared<Job>(fun); }

    explicit Job(std::function<void()> fun);

    void execute(JobPtr_t& /*thisJobPtr*/, ThreadPool& /*pool*/) override;

    std::function<void()> getFunction() { return m_fun; }

    void waitForTermination();

private:
    std::function<void()> m_fun;
    std::mutex            m_terminationMutex;
};

/**
 * @brief A recurring job which can be cancelled manually.
 *
 */
class RecurringJob : public Job {
public:
    static JobPtr_t create(std::function<void()> fun) {
        return std::make_shared<RecurringJob>(fun);
    }

    void execute(JobPtr_t& thisJobPtr, ThreadPool& pool) override;

    void cancel() { m_isCancelled = true; }

    using Job::Job;

private:
    bool m_isCancelled{false};
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_JOB_H
