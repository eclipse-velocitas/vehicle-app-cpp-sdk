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

#include <gtest/gtest.h>

#include <condition_variable>
#include <mutex>
#include <thread>

using namespace velocitas;
using namespace std::chrono_literals;

TEST(Test_Job, callCtor_success) {
    EXPECT_NO_THROW(Job([] {}));
}

TEST(Test_Job, create_firstCall_returnsNonNullPtr) {
    JobPtr_t job;
    EXPECT_NO_THROW(job = Job::create([] {}));
    EXPECT_TRUE(job);
}

TEST(Test_Job, create_subsequentCall_returnsDiffernentNonNullPtr) {
    JobPtr_t job1 = Job::create([] {});

    JobPtr_t job2;
    EXPECT_NO_THROW(job2 = Job::create([] {}));
    EXPECT_TRUE(job2);
    EXPECT_NE(job1.get(), job2.get());
}

TEST(Test_Job, shallRecur_returnsFalse) {
    Job job([] {});
    EXPECT_FALSE(job.shallRecur());
}

TEST(Test_Job, createJob_passedFunctionIsNotExecuted) {
    bool executeWasCalled = false;
    Job  job([&executeWasCalled] { executeWasCalled = true; });
    EXPECT_FALSE(executeWasCalled);
}

TEST(Test_Job, execute_passedFunctionIsExecuted) {
    bool executeWasCalled = false;
    Job  job([&executeWasCalled] { executeWasCalled = true; });

    job.execute();
    EXPECT_TRUE(executeWasCalled);
}

TEST(Test_Job, waitForTermination_notExecuting_returnsImmediately) {
    bool executeWasCalled = false;
    Job  job([&executeWasCalled] { executeWasCalled = true; });

    EXPECT_NO_THROW(job.waitForTermination());
}

TEST(Test_Job, waitForTermination_executing_doesNotReturnWhileExecuting) {
    bool                    isExecuting          = false;
    bool                    shallFinishExecution = false;
    std::mutex              executorMutex;
    std::condition_variable executorCV;
    Job                     job([&] {
        {
            std::lock_guard guard{executorMutex};
            isExecuting = true;
            executorCV.notify_one();
        }
        {
            std::unique_lock lock(executorMutex);
            executorCV.wait(lock, [&] { return shallFinishExecution; });
        }
    });
    std::thread             executor([&] { job.execute(); });

    // make sure job is in execution
    {
        std::unique_lock lock(executorMutex);
        executorCV.wait(lock, [&] { return isExecuting; });
    }

    // setup the scavenger (waiting for termination of the job)
    enum WaitState { Initial, IsWaiting, HasFinishedWaiting };
    WaitState               waitState = Initial;
    std::mutex              scavengerMutex;
    std::condition_variable scavengerCV;
    std::thread             scavenger([&] {
        {
            std::lock_guard guard{scavengerMutex};
            waitState = IsWaiting;
        }
        scavengerCV.notify_one();
        job.waitForTermination();
        {
            std::lock_guard guard{scavengerMutex};
            waitState = HasFinishedWaiting; // cppcheck-suppress redundantAssignment
        }
    });

    // make sure scavenger is waiting
    {
        std::unique_lock lock{scavengerMutex};
        scavengerCV.wait(lock, [&] { return waitState == IsWaiting; });
        std::this_thread::sleep_for(2ms);

        EXPECT_EQ(IsWaiting, waitState);
    }

    // Notify executor to finish
    {
        std::lock_guard guard{executorMutex};
        shallFinishExecution = true;
        executorCV.notify_one();
    }
    executor.join();
    scavenger.join();
}

TEST(Test_Job, finishExecution_waitingForTermination_returnsImmediatly) {
    bool                    isExecuting          = false;
    bool                    shallFinishExecution = false;
    std::mutex              executorMutex;
    std::condition_variable executorCV;
    Job                     job([&] {
        {
            std::lock_guard guard{executorMutex};
            isExecuting = true;
            executorCV.notify_one();
        }
        {
            std::unique_lock lock(executorMutex);
            executorCV.wait(lock, [&] { return shallFinishExecution; });
        }
    });
    std::thread             executor([&] {
        job.execute();
        {
            std::lock_guard guard{executorMutex};
            isExecuting = false;
            executorCV.notify_one();
        }
    });

    // make sure job is in execution
    {
        std::unique_lock lock(executorMutex);
        executorCV.wait(lock, [&] { return isExecuting; });
    }

    // setup the scavenger (waiting for termination of the job)
    enum WaitState { Initial, IsWaiting, HasFinishedWaiting };
    WaitState               waitState = Initial;
    std::mutex              scavengerMutex;
    std::condition_variable scavengerCV;
    std::thread             scavenger([&] {
        {
            std::lock_guard guard{scavengerMutex};
            waitState = IsWaiting;
        }
        scavengerCV.notify_one();
        job.waitForTermination();
        {
            std::lock_guard guard{scavengerMutex};
            waitState = HasFinishedWaiting; // cppcheck-suppress redundantAssignment
        }
        scavengerCV.notify_one();
    });

    // make sure scavenger is waiting
    {
        std::unique_lock lock{scavengerMutex};
        scavengerCV.wait(lock, [&] { return waitState == IsWaiting; });
        std::this_thread::sleep_for(2ms);
    }

    // Notify executor to finish
    {
        std::lock_guard guard{executorMutex};
        shallFinishExecution = true;
        executorCV.notify_one();
    }

    // Wait till executor is done
    {
        std::unique_lock lock{executorMutex};
        executorCV.wait(lock, [&] { return !isExecuting; });
    }

    // make sure scavenger is done
    {
        std::unique_lock lock{scavengerMutex};
        scavengerCV.wait(lock, [&] { return HasFinishedWaiting == waitState; });
        EXPECT_EQ(HasFinishedWaiting, waitState);
    }

    executor.join();
    scavenger.join();
}

TEST(Test_RecurringJob, callCtor_success) {
    EXPECT_NO_THROW(RecurringJob([] {}));
}

TEST(Test_RecurringJob, create_firstCall_returnsNonNullPtr) {
    JobPtr_t job;
    EXPECT_NO_THROW(job = RecurringJob::create([] {}));
    EXPECT_TRUE(job);
}

TEST(Test_RecurringJob, create_subsequentCall_returnsDifferentNonNullPtr) {
    JobPtr_t job1 = RecurringJob::create([] {});

    JobPtr_t job2;
    EXPECT_NO_THROW(job2 = RecurringJob::create([] {}));
    EXPECT_TRUE(job2);
    EXPECT_NE(job1.get(), job2.get());
}

TEST(Test_RecurringJob, shallRecur_freshlyCreated_returnsTrue) {
    RecurringJob job([] {});
    EXPECT_TRUE(job.shallRecur());
}

TEST(Test_RecurringJob, cancel_freshlyCreated_success) {
    RecurringJob job([] {});
    EXPECT_NO_THROW(job.cancel());
}

TEST(Test_RecurringJob, shallRecur_cancelled_returnsFalse) {
    RecurringJob job([] {});
    job.cancel();
    EXPECT_FALSE(job.shallRecur());
}

TEST(Test_RecurringJob, createRecurringJob_passedFunctionIsNotExecuted) {
    bool         executeWasCalled = false;
    RecurringJob job([&executeWasCalled] { executeWasCalled = true; });
    EXPECT_FALSE(executeWasCalled);
}

TEST(Test_Job, execute_notCancelled_passedFunctionIsExecuted) {
    bool         executeWasCalled = false;
    RecurringJob job([&executeWasCalled] { executeWasCalled = true; });

    job.execute();
    EXPECT_TRUE(executeWasCalled);
}

TEST(Test_Job, execute_cancelled_passedFunctionIsNotExecuted) {
    bool         executeWasCalled = false;
    RecurringJob job([&executeWasCalled] { executeWasCalled = true; });
    job.cancel();

    job.execute();
    EXPECT_FALSE(executeWasCalled);
}
