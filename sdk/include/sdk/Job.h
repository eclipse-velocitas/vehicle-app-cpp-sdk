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

#ifndef VEHICLE_APP_SDK_JOB_H
#define VEHICLE_APP_SDK_JOB_H

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>

namespace velocitas {

using Clock     = std::chrono::steady_clock;
using Timepoint = std::chrono::time_point<Clock>;

/**
 * @brief Interface for jobs which can be executed by a worker in the thread pool.
 */
class IJob {
public:
    IJob()          = default;
    virtual ~IJob() = default;

    [[nodiscard]] virtual bool isDue() const { return true; }

    [[nodiscard]] virtual Timepoint getTimepointToExecute() const { return {}; }

    /**
     * @brief Execute the job.
     */
    virtual void execute() = 0;

    /**
     * @brief Indicates if this job shall recur after its execution.
     *
     * @return true - recur this job
     * @return false - don't recure
     */
    [[nodiscard]] virtual bool shallRecur() const { return false; }

    IJob(const IJob&)            = delete;
    IJob(IJob&&)                 = delete;
    IJob& operator=(const IJob&) = delete;
    IJob& operator=(IJob&&)      = delete;
};

using JobPtr_t = std::shared_ptr<IJob>;

/**
 * @brief A nonrecurring job.
 */
class Job : public IJob {
public:
    static JobPtr_t create(std::function<void()>     fun,
                           std::chrono::milliseconds delay = std::chrono::milliseconds::zero()) {
        return std::make_shared<Job>(fun, delay);
    }

    explicit Job(std::function<void()>     fun,
                 std::chrono::milliseconds delay = std::chrono::milliseconds::zero());

    bool isDue() const override {
        return (m_timepointToExecute == Timepoint()) || (m_timepointToExecute <= Clock::now());
    }

    Timepoint getTimepointToExecute() const override { return m_timepointToExecute; }

    void execute() override;

    void waitForTermination() const;

private:
    std::function<void()> m_fun;
    Timepoint             m_timepointToExecute;
    mutable std::mutex    m_terminationMutex;
};

bool lowerJobPriority(const JobPtr_t& left, const JobPtr_t& right);

/**
 * @brief A recurring job which can be cancelled manually.
 */
class RecurringJob : public Job {
public:
    static JobPtr_t create(std::function<void()> fun) {
        return std::make_shared<RecurringJob>(fun);
    }

    using Job::Job;

    void execute() override;

    /**
     * @brief Prevents execution of the function once called.
     */
    void cancel() { m_isCancelled = true; }

    [[nodiscard]] bool shallRecur() const override { return !m_isCancelled; }

private:
    std::atomic_bool m_isCancelled{false};
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_JOB_H
