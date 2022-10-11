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

#ifndef VEHICLE_APP_SDK_EXCEPTIONS_H
#define VEHICLE_APP_SDK_EXCEPTIONS_H

#include <stdexcept>

namespace velocitas {

/**
 * @brief Base exception when there is an issue during remote procedure calls.
 *
 */
class RpcException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    ~RpcException() override = default;

    RpcException(const RpcException& other)            = default;
    RpcException(RpcException&& other)                 = default;
    RpcException& operator=(const RpcException& other) = default;
    RpcException& operator=(RpcException&& other)      = default;
};

/**
 * @brief Base exception when a type cannot be converted.
 *
 */
class InvalidTypeException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    ~InvalidTypeException() override = default;

    InvalidTypeException(const InvalidTypeException& other)            = default;
    InvalidTypeException(InvalidTypeException&& other)                 = default;
    InvalidTypeException& operator=(const InvalidTypeException& other) = default;
    InvalidTypeException& operator=(InvalidTypeException&& other)      = default;
};

/**
 * @brief Base exception when an invalid value is received.
 *
 */
class InvalidValueException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    ~InvalidValueException() override = default;

    InvalidValueException(const InvalidValueException& other)            = default;
    InvalidValueException(InvalidValueException&& other)                 = default;
    InvalidValueException& operator=(const InvalidValueException& other) = default;
    InvalidValueException& operator=(InvalidValueException&& other)      = default;
};

/**
 * @brief Any issue which occurred during async invocation.
 *
 */
class AsyncException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    ~AsyncException() override = default;

    AsyncException(const AsyncException& other)            = default;
    AsyncException(AsyncException&& other)                 = default;
    AsyncException& operator=(const AsyncException& other) = default;
    AsyncException& operator=(AsyncException&& other)      = default;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_EXCEPTIONS_H
