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
#include <string>

namespace velocitas {

/**
 * @brief Base exception when there is an issue during remote procedure calls.
 *
 */
class RpcException : public std::runtime_error {
public:
    explicit RpcException(const std::string& message)
        : std::runtime_error(message) {}
    ~RpcException() override = default;

    RpcException(const RpcException& other)
        : std::runtime_error(other) {}
    RpcException(RpcException&& other)
        : std::runtime_error(other) {}

    RpcException& operator=(const RpcException& other) {
        std::runtime_error::operator=(other);
        return *this;
    }

    RpcException& operator=(RpcException&& other) {
        std::runtime_error::operator=(other);
        return *this;
    }
};

/**
 * @brief Base exception when a type cannot be converted.
 *
 */
class InvalidTypeException : public std::runtime_error {
public:
    explicit InvalidTypeException(const std::string& message)
        : std::runtime_error(message) {}
    ~InvalidTypeException() override = default;

    InvalidTypeException(const InvalidTypeException& other)
        : std::runtime_error(other) {}
    InvalidTypeException(InvalidTypeException&& other)
        : std::runtime_error(other) {}

    InvalidTypeException& operator=(const InvalidTypeException& other) {
        std::runtime_error::operator=(other);
        return *this;
    }

    InvalidTypeException& operator=(InvalidTypeException&& other) {
        std::runtime_error::operator=(other);
        return *this;
    }
};

/**
 * @brief Base exception when an invalid value is received.
 *
 */
class InvalidValueException : public std::runtime_error {
public:
    explicit InvalidValueException(const std::string& msg)
        : std::runtime_error(msg) {}
    ~InvalidValueException() override = default;

    InvalidValueException(const InvalidValueException& other)
        : std::runtime_error(other) {}
    InvalidValueException(InvalidValueException&& other)
        : std::runtime_error(other) {}

    InvalidValueException& operator=(const InvalidValueException& other) {
        std::runtime_error::operator=(other);
        return *this;
    }

    InvalidValueException& operator=(InvalidValueException&& other) {
        std::runtime_error::operator=(other);
        return *this;
    }
};

/**
 * @brief Any issue which occurred during async invocation.
 *
 */
class AsyncException : public std::runtime_error {
public:
    explicit AsyncException(const std::string& message)
        : std::runtime_error(message) {}
    ~AsyncException() override = default;

    AsyncException(const AsyncException& other)
        : std::runtime_error(other) {}
    AsyncException(AsyncException&& other)
        : std::runtime_error(other) {}

    AsyncException& operator=(const AsyncException& other) {
        std::runtime_error::operator=(other);
        return *this;
    }

    AsyncException& operator=(AsyncException&& other) {
        std::runtime_error::operator=(other);
        return *this;
    }
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_EXCEPTIONS_H
