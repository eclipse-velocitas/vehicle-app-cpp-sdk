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

#include <atomic>
#include <exception>
#include <gtest/gtest.h>

using namespace velocitas;
using namespace std::chrono_literals;

static const std::chrono::milliseconds DEFAULT_TIMEOUT = 10s;

class FakeJob : public velocitas::IJob {
public:
    enum State { Initialized, Executing, Finished };

    void execute() override {
        {
            std::lock_guard lock(m_executionMutex);
            m_executionState = Executing;
            m_executionCount++;
            m_executionStateCV.notify_all();
        }
        {
            std::unique_lock lock{m_stopMutex};
            m_stopNotifier.wait(lock, [this] { return m_stopJob; });
        }
        {
            std::lock_guard lock(m_executionMutex);
            m_executionState = Finished;
            m_stopJob        = false;
            m_executionStateCV.notify_all();
        }
        if (m_throwException) {
            throw std::exception();
        }
    }
    void finish() {
        std::lock_guard lock(m_stopMutex);
        m_stopJob = true;
        m_stopNotifier.notify_one();
    }
    void throwException() {
        m_throwException = true;
        finish();
    }
    bool waitForExecution(const std::chrono::milliseconds& timeout = DEFAULT_TIMEOUT) {
        std::unique_lock lock{m_executionMutex};
        return m_executionStateCV.wait_for(lock, timeout,
                                           [this] { return m_executionState == Executing; });
    }
    bool waitForFinished(const std::chrono::milliseconds& timeout = DEFAULT_TIMEOUT) {
        std::unique_lock lock{m_executionMutex};
        return m_executionStateCV.wait_for(lock, timeout,
                                           [this] { return m_executionState == Finished; });
    }
    [[nodiscard]] State getExecutionState() const { return m_executionState; }

    std::atomic_uint        m_executionCount{0};
    std::atomic<State>      m_executionState{Initialized};
    std::mutex              m_executionMutex;
    std::condition_variable m_executionStateCV;
    bool                    m_stopJob{false};
    bool                    m_throwException{false};
    std::mutex              m_stopMutex;
    std::condition_variable m_stopNotifier;
};

using FakeJobPtr_t = std::shared_ptr<FakeJob>;

class FakeRecurringJob : public FakeJob {
public:
    [[nodiscard]] bool shallRecur() const override { return m_doRecur; }
    void               cancel() { m_doRecur = false; }

    std::atomic_bool m_doRecur{true};
};

class Test_ThreadPool : public ::testing::Test {
protected:
    void SetUp() override { m_pool = std::make_shared<ThreadPool>(); }

    void TearDown() override { stopCreatedJobs(); }

    bool createAndExecuteJob(const std::chrono::milliseconds& timeout = DEFAULT_TIMEOUT) {
        auto job = std::make_shared<FakeJob>();
        m_fakeJobs.push(job);
        m_pool->enqueue(job);
        return job->waitForExecution(timeout);
    }

    bool finishJob(FakeJobPtr_t job, const std::chrono::milliseconds& timeout = DEFAULT_TIMEOUT) {
        job->finish();
        return job->waitForFinished(timeout);
    }

    void stopCreatedJobs() {
        while (!m_fakeJobs.empty()) {
            m_fakeJobs.front()->finish();
            m_fakeJobs.pop();
        }
    }

    bool occupyAllWorkers() {
        for (size_t jobNum = 0; jobNum < m_pool->getNumWorkerThreads(); ++jobNum) {
            if (!createAndExecuteJob()) {
                return false;
            }
        }
        return true;
    }

    std::shared_ptr<ThreadPool> m_pool;
    std::queue<FakeJobPtr_t>    m_fakeJobs;
};

TEST_F(Test_ThreadPool, getInstance_returnsNonNullPointer) {
    auto pool = ThreadPool::getInstance();
    EXPECT_NE(pool, nullptr);
}

TEST_F(Test_ThreadPool, getInstance_multipleCalls_returnsSameInstance) {
    auto pool1 = ThreadPool::getInstance();
    auto pool2 = ThreadPool::getInstance();
    EXPECT_EQ(pool1, pool2);
}

TEST_F(Test_ThreadPool, callThreadPoolCtr_success) { EXPECT_NO_THROW(ThreadPool()); }

TEST_F(Test_ThreadPool, enqueue_firstJob_jobExecuting) {
    auto job = std::make_shared<FakeJob>();
    ASSERT_EQ(FakeJob::Initialized, job->getExecutionState());

    m_pool->enqueue(job);
    EXPECT_TRUE(job->waitForExecution());
    EXPECT_EQ(FakeJob::Executing, job->getExecutionState());

    job->finish();
}

TEST_F(Test_ThreadPool, enqueueOccupyingAllWorkers_noActiveWorkers_allJobExecuting) {
    EXPECT_TRUE(occupyAllWorkers());
}

TEST_F(Test_ThreadPool, enqueue_allWorkersOccupied_jobNotExecuted) {
    ASSERT_TRUE(occupyAllWorkers());

    EXPECT_FALSE(createAndExecuteJob(10ms));
}

TEST_F(Test_ThreadPool, stopExecutingOneJob_queuedJob_jobExecuted) {
    ASSERT_TRUE(occupyAllWorkers());
    ASSERT_FALSE(createAndExecuteJob(0ms));

    ASSERT_TRUE(finishJob(m_fakeJobs.front()));
    EXPECT_TRUE(m_fakeJobs.back()->waitForExecution());
}

TEST_F(Test_ThreadPool, destroyThreadPool_occupiedWorkers_waitForWorkersFinished) {
    ASSERT_TRUE(occupyAllWorkers());

    std::thread poolKiller([this] {
        // use own thread here as the pool d-tor won't return before all executing jobs are
        m_pool.reset();
    });

    while (!m_fakeJobs.empty()) {
        EXPECT_TRUE(finishJob(m_fakeJobs.front()));
        m_fakeJobs.pop();
    }
    EXPECT_NO_THROW(poolKiller.join());
}

TEST_F(Test_ThreadPool, destroyThreadPool_oneQueuedJob_cleanlyTerminates) {
    ASSERT_TRUE(occupyAllWorkers());
    auto queuedJob = std::make_shared<FakeJob>();
    m_pool->enqueue(queuedJob);
    m_fakeJobs.push(queuedJob);

    std::thread poolKiller([this] {
        // use own thread here as the pool d-tor won't return before all executing jobs are
        m_pool.reset();
    });
    stopCreatedJobs();
    EXPECT_NO_THROW(poolKiller.join());
}

TEST_F(Test_ThreadPool, finishJob_jobExecuting_jobNotExecutedAgain) {
    auto job = std::make_shared<FakeJob>();
    m_pool->enqueue(job);
    ASSERT_TRUE(job->waitForExecution());

    EXPECT_TRUE(finishJob(job));
    EXPECT_FALSE(job->waitForExecution(10ms));
    EXPECT_EQ(1, job->m_executionCount);
}

TEST_F(Test_ThreadPool, finishRecurringJob_jobExecutingAndCancelled_jobNotExecutedAgain) {
    auto job = std::make_shared<FakeRecurringJob>();
    m_pool->enqueue(job);
    ASSERT_TRUE(job->waitForExecution());
    job->cancel();

    EXPECT_TRUE(finishJob(job));
    EXPECT_FALSE(job->waitForExecution(10ms));
    EXPECT_EQ(1, job->m_executionCount);
}

TEST_F(Test_ThreadPool, finishRecurringJob_jobExecuting_jobIsExecutedAgain) {
    auto job = std::make_shared<FakeRecurringJob>();
    m_pool->enqueue(job);
    ASSERT_TRUE(job->waitForExecution());

    finishJob(job);
    EXPECT_TRUE(job->waitForExecution(10ms));
    EXPECT_EQ(2, job->m_executionCount);

    job->cancel();
    job->finish();
}

TEST_F(Test_ThreadPool, jobThrows_executing_workerNotTerminated) {
    ASSERT_TRUE(createAndExecuteJob());
    ASSERT_FALSE(m_fakeJobs.empty());
    m_fakeJobs.front()->throwException();

    // It should be still possible to get jobs executed on all (initial) workers
    EXPECT_TRUE(occupyAllWorkers());
}
