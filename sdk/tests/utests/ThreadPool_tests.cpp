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

#include <atomic>
#include <gtest/gtest.h>

#include "sdk/ThreadPool.h"

using namespace velocitas;
using namespace std::chrono_literals;

static const unsigned int DEFAULT_TIMEOUT = 60 * 000; // 1 min.

class MockJob : public velocitas::IJob {
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
    }
    void finish() {
        std::lock_guard lock(m_stopMutex);
        m_stopJob = true;
        m_stopNotifier.notify_one();
    }
    bool waitForExecution(unsigned int timeoutInMs) {
        std::unique_lock lock{m_executionMutex};
        if (timeoutInMs == 0) {
            timeoutInMs = DEFAULT_TIMEOUT;
        }
        return m_executionStateCV.wait_for(lock, timeoutInMs * 1ms,
                                           [this] { return m_executionState == Executing; });
    }
    bool waitForFinished(unsigned int timeoutInMs) {
        std::unique_lock lock{m_executionMutex};
        if (timeoutInMs == 0) {
            timeoutInMs = DEFAULT_TIMEOUT;
        }
        return m_executionStateCV.wait_for(lock, timeoutInMs * 1ms,
                                           [this] { return m_executionState == Finished; });
    }
    [[nodiscard]] State getExecutionState() const { return m_executionState; }

    std::atomic_uint        m_executionCount{0};
    std::atomic<State>      m_executionState{Initialized};
    std::mutex              m_executionMutex;
    std::condition_variable m_executionStateCV;
    bool                    m_stopJob{false};
    std::mutex              m_stopMutex;
    std::condition_variable m_stopNotifier;
};

using MockJobPtr_t = std::shared_ptr<MockJob>;

class MockRecurringJob : public MockJob {
public:
    [[nodiscard]] bool shallRecur() const override { return m_doRecur; }
    void               cancel() { m_doRecur = false; }

    std::atomic_bool m_doRecur{true};
};

class Test_ThreadPool : public ::testing::Test {
protected:
    void SetUp() override { m_pool = std::make_shared<ThreadPool>(); }

    void TearDown() override { stopCreatedJobs(); }

    bool createAndExecuteJob(unsigned int timeoutInMs = 0) {
        auto job = std::make_shared<MockJob>();
        m_mockJobs.push(job);
        m_pool->enqueue(job);
        return job->waitForExecution(timeoutInMs);
    }

    bool finishJob(MockJobPtr_t job, unsigned int timeoutInMs) {
        job->finish();
        return job->waitForFinished(timeoutInMs);
    }

    void stopCreatedJobs() {
        while (!m_mockJobs.empty()) {
            m_mockJobs.front()->finish();
            m_mockJobs.pop();
        }
    }

    bool occupyAllWorkers() {
        for (size_t jobNum = 0; jobNum < m_pool->getNumWorkerThreads(); ++jobNum) {
            if (!createAndExecuteJob(10)) {
                return false;
            }
        }
        return true;
    }

    std::shared_ptr<ThreadPool> m_pool;
    std::queue<MockJobPtr_t>    m_mockJobs;
};

TEST_F(Test_ThreadPool, getInstance_none_noFailureAndNoNullPointer) {
    auto pool = ThreadPool::getInstance();
    EXPECT_NE(pool, nullptr);
}

TEST_F(Test_ThreadPool, getInstance_secondTime_returnsSameInstance) {
    auto pool1 = ThreadPool::getInstance();
    auto pool2 = ThreadPool::getInstance();
    EXPECT_EQ(pool1, pool2);
}

TEST_F(Test_ThreadPool, callThreadPoolCtr_none_noFailure) { EXPECT_TRUE(m_pool); }

TEST_F(Test_ThreadPool, enqueue_firstJob_jobExecuting) {
    auto job = std::make_shared<MockJob>();
    ASSERT_EQ(MockJob::Initialized, job->getExecutionState());

    m_pool->enqueue(job);
    EXPECT_TRUE(job->waitForExecution(2));
    EXPECT_EQ(MockJob::Executing, job->getExecutionState());

    job->finish();
}

TEST_F(Test_ThreadPool, enqueueOccupyingAllWorkers_noActiveWorkers_allJobExecuting) {
    EXPECT_TRUE(occupyAllWorkers());
}

TEST_F(Test_ThreadPool, enqueue_allWorkersOccupied_jobNotExectuted) {
    ASSERT_TRUE(occupyAllWorkers());

    EXPECT_FALSE(createAndExecuteJob(10));
}

TEST_F(Test_ThreadPool, stopExecutingOneJob_queuedJob_jobExectuted) {
    ASSERT_TRUE(occupyAllWorkers());
    ASSERT_FALSE(createAndExecuteJob(0));

    auto job = m_mockJobs.front();
    ASSERT_TRUE(finishJob(m_mockJobs.front(), 2));
    EXPECT_TRUE(m_mockJobs.back()->waitForExecution(2));
}

TEST_F(Test_ThreadPool, stopExecutingJob_queuedEmptyJobAndJob_emptyJobIgnoredAndJobExectuted) {
    ASSERT_TRUE(occupyAllWorkers());
    m_pool->enqueue(nullptr);
    ASSERT_FALSE(createAndExecuteJob(0));

    auto job = m_mockJobs.front();
    ASSERT_TRUE(finishJob(m_mockJobs.front(), 2));
    EXPECT_TRUE(m_mockJobs.back()->waitForExecution(2));
}

TEST_F(Test_ThreadPool, destroyThreadPool_occupiedWorkers_waitForWorkersFinished) {
    ASSERT_TRUE(occupyAllWorkers());

    std::thread poolKiller([this] {
        // use own thread here as the pool d-tor won't return before all executing jobs are
        m_pool.reset();
    });

    while (!m_mockJobs.empty()) {
        EXPECT_TRUE(finishJob(m_mockJobs.front(), 2));
        m_mockJobs.pop();
    }
    EXPECT_NO_THROW(poolKiller.join());
}

TEST_F(Test_ThreadPool, destroyThreadPool_oneQueuedJob_cleanlyTerminates) {
    ASSERT_TRUE(occupyAllWorkers());
    auto queuedJob = std::make_shared<MockJob>();
    m_pool->enqueue(queuedJob);
    m_mockJobs.push(queuedJob);

    std::thread poolKiller([this] {
        // use own thread here as the pool d-tor won't return before all executing jobs are
        m_pool.reset();
    });
    stopCreatedJobs();
    EXPECT_NO_THROW(poolKiller.join());
}

TEST_F(Test_ThreadPool, finishJob_jobExecuting_jobNotExecutedAgain) {
    auto job = std::make_shared<MockJob>();
    m_pool->enqueue(job);
    ASSERT_TRUE(job->waitForExecution(2));

    EXPECT_TRUE(finishJob(job, 2));
    EXPECT_FALSE(job->waitForExecution(10));
    EXPECT_EQ(1, job->m_executionCount);
}

TEST_F(Test_ThreadPool, finishRecurringJob_jobExecutingAndCancelled_jobNotExecutedAgain) {
    auto job = std::make_shared<MockRecurringJob>();
    m_pool->enqueue(job);
    ASSERT_TRUE(job->waitForExecution(2));
    job->cancel();

    EXPECT_TRUE(finishJob(job, 2));
    EXPECT_FALSE(job->waitForExecution(10));
    EXPECT_EQ(1, job->m_executionCount);
}

TEST_F(Test_ThreadPool, finishRecurringJob_jobExecuting_jobIsExecutedAgain) {
    auto job = std::make_shared<MockRecurringJob>();
    m_pool->enqueue(job);
    ASSERT_TRUE(job->waitForExecution(2));

    finishJob(job, 2);
    EXPECT_TRUE(job->waitForExecution(10));
    EXPECT_EQ(2, job->m_executionCount);

    job->cancel();
    job->finish();
}
