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

#ifndef VEHICLE_APP_SDK_ASYNCRESULT_H
#define VEHICLE_APP_SDK_ASYNCRESULT_H

#include "sdk/Exceptions.h"
#include "sdk/Status.h"

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

namespace velocitas {

/**
 * @brief Flips a boolean value within a scope.
 *
 */
class ScopedBoolInverter {
public:
    explicit ScopedBoolInverter(bool& var) // NOLINT
        : m_varPtr(&var) {
        *m_varPtr = !*m_varPtr;
    }
    ~ScopedBoolInverter() { *m_varPtr = !*m_varPtr; }

    ScopedBoolInverter(const ScopedBoolInverter&) = delete;
    ScopedBoolInverter(ScopedBoolInverter&&)      = delete;

    ScopedBoolInverter& operator=(const ScopedBoolInverter&) = delete;
    ScopedBoolInverter& operator=(ScopedBoolInverter&&)      = delete;

private:
    bool* m_varPtr;
};

/**
 * @brief Empty result structure which can be used in case an AsyncResult
 *        does not return any valuable information other than its success.
 *
 */
struct VoidResult {};

enum class CallState { ONGOING, CANCELING, COMPLETED, FAILED };

/**
 * @brief Single result of an asynchronous operation which provides
 *        an item of type TResultType.
 *
 * @tparam TResultType  Result type of the async operation.
 */
template <typename TResultType> class AsyncResult {
public:
    using ResultCallback_t = std::function<void(const TResultType&)>;
    using ErrorCallback_t  = std::function<void(Status)>;

    AsyncResult() {
        // initially lock the mutex until we have written the result.
        // this will cause await() to block until the result is available.
        m_mutex.lock();
    };

    /**
     * @brief Inserts the result and notifies any waiters.
     *
     * @param result  Result to insert.
     */
    void insertResult(TResultType&& result) {
        m_callState = CallState::COMPLETED;
        if (m_callback != nullptr) {
            m_callback(result);
        } else {
            m_result = std::move(result);
        }
        m_mutex.unlock();
    }

    /**
     * @brief Inserts a new error and notifies any waiters.
     *
     * @param error Status containing error information.
     */
    void insertError(Status&& error) {
        m_callState = CallState::FAILED;
        if (m_errorCallback != nullptr) {
            m_errorCallback(error);
        } else {
            m_status = error;
        }
        m_mutex.unlock();
    }

    /**
     * @brief Blocks the calling thread until the result is available.
     *
     * @throw AsyncException     if there is any issues during async invocation.
     * @throw std::runtime_error if the API usage is wrong.
     *
     * @return TResultType    Result of the async operation once it completes.
     */
    TResultType await() {
        if (m_callback) {
            throw std::runtime_error(
                "Invalid usage: Either call await() or register an onResult callback!");
        }

        {
            ScopedBoolInverter           boolInverter{m_awaiting};
            std::unique_lock<std::mutex> lock(m_mutex);

            if (m_status.ok()) {
                return m_result;
            }
            throw AsyncException(m_status.errorMessage());
        }
    }

    /**
     * @brief Calls the specified callback when the result is available.
     *        The callback invocation is done by a worker thread.
     *
     * @param callback      The callback to invoke.
     * @return AsyncResult* This for method chaining.
     */
    AsyncResult* onResult(ResultCallback_t callback) {
        if (m_awaiting) {
            throw std::runtime_error(
                "Invalid usage: Either call await() or register an onResult callback!");
        }
        m_callback = callback;
        if (m_callState == CallState::COMPLETED) {
            m_callback(m_result);
        }
        return this;
    }

    /**
     * @brief Calls the specified callback when an error occurrs during async execution.
     *        The callback invocation is done by a worker thread.
     *
     * @param callback      The callback to invoke.
     * @return AsyncResult* This for method chaining.
     */
    AsyncResult* onError(ErrorCallback_t callback) {
        m_errorCallback = callback;
        if (m_callState == CallState::FAILED) {
            m_errorCallback(m_status);
        }
        return this;
    }

    /**
     * @brief Return if the result is currently being awaited.
     *
     * @return true
     * @return false
     */
    [[nodiscard]] bool isInAwaitingState() const { return m_awaiting; }

    /**
     * @brief Map the result to a different type using the provided mapper function.
     *
     * @tparam TNewType    The new type created by the mapper function.
     * @param mapper       The mapper function to convert TResultType to TNewType.
     * @return std::shared_ptr<AsyncResult<TNewType>>
     *                     A new AsyncResult object which emits results of TNewType.
     */
    template <typename TNewType>
    std::shared_ptr<AsyncResult<TNewType>> map(std::function<TNewType(const TResultType&)> mapper) {
        auto mappedResult = std::make_shared<AsyncResult<TNewType>>();

        onResult([mappedResult, mapper](auto item) { mappedResult->insertResult(mapper(item)); });

        onError([mappedResult](auto status) { mappedResult->insertError(std::move(status)); });

        if (m_mutex.try_lock()) {
            // result already available?!
            if (m_status.ok()) {
                mappedResult->insertResult(mapper(m_result));
            } else {
                mappedResult->insertError(std::move(m_status));
            }
        }

        return mappedResult;
    }

private:
    CallState        m_callState{CallState::ONGOING};
    TResultType      m_result;
    ResultCallback_t m_callback;
    ErrorCallback_t  m_errorCallback;
    std::mutex       m_mutex;
    bool             m_awaiting{false};
    Status           m_status{};
};

template <typename T> using AsyncResultPtr_t = std::shared_ptr<AsyncResult<T>>;

/**
 * @brief An asynchronous subscription to a data source which provides
 *        items of type TResultType.
 *
 * @tparam TResultType  Item type of the async subscription.
 */
template <typename TResultType> class AsyncSubscription {
public:
    using ItemCallback_t  = std::function<void(const TResultType&)>;
    using ErrorCallback_t = std::function<void(Status)>;

    AsyncSubscription() noexcept = default;

    /**
     * @brief Blocks the calling thread until the next item is available
     *        and returns it.
     * @throw AsyncException if there is any issues during async invocation.
     */
    TResultType next() {
        std::unique_lock<std::mutex> lock(m_bufferMutex);
        if (m_bufferedItems.empty()) {
            m_cv.wait(lock);
        }

        if (m_status.ok()) {
            auto temp = m_bufferedItems.back();
            m_bufferedItems.pop_back();
            return temp;
        }
        throw AsyncException(m_status.errorMessage());
    }

    /**
     * @brief Calls the specified callback whenever a new item is available.
     *        The callback invocation is done by a worker thread.
     *
     * @param callback              The callback to invoke.
     * @return AsyncSubscription*   This subscription for method chaining.
     */
    AsyncSubscription* onItem(ItemCallback_t callback) {
        m_callback = callback;
        return this;
    }

    /**
     * @brief Calls the specified callback when an error occurs.
     *        Once an error occurs, the subscription is cancelled by default.
     *        The callback invocation is done by a worker thread.
     *
     * @param errorCallback         The callback to invoke.
     * @return AsyncSubscription*   This subscription for method chaining.
     */
    AsyncSubscription* onError(ErrorCallback_t errorCallback) {
        m_errorCallback = errorCallback;
        return this;
    }

    /**
     * @brief Inserts new data into the subscription. Notifies any waiters.
     *
     * @param result  Result to insert.
     */
    void insertNewItem(TResultType&& result) {
        if (m_callback != nullptr) {
            m_callback(result);
        } else {
            {
                std::lock_guard<std::mutex> lock(m_bufferMutex);
                m_bufferedItems.insert(m_bufferedItems.begin(), result);
            }
            m_cv.notify_all();
        }
    }

    /**
     * @brief Inserts a new error into the subscription. Notifies any waiters.
     *
     * @param error Status with error information.
     */
    void insertError(Status&& error) {
        if (m_errorCallback != nullptr) {
            m_errorCallback(error);
        } else {
            m_status = error;
            m_cv.notify_all();
        }
    }

    /**
     * @brief Cancels the subscription.
     *
     */
    void cancel() { m_cancelled = true; }

private:
    std::vector<TResultType> m_bufferedItems;
    ItemCallback_t           m_callback;
    ErrorCallback_t          m_errorCallback;
    std::mutex               m_bufferMutex;
    bool                     m_cancelled{false};
    Status                   m_status{};
    std::condition_variable  m_cv;
};

template <typename T> using AsyncSubscriptionPtr_t = std::shared_ptr<AsyncSubscription<T>>;

} // namespace velocitas

#endif // VEHICLE_APP_SDK_ASYNCRESULT_H
