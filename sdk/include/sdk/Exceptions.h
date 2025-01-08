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
};

/**
 * @brief Base exception when a type cannot be converted.
 *
 */
class InvalidTypeException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/**
 * @brief Base exception when an invalid value is received.
 *
 */
class InvalidValueException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/**
 * @brief Any issue which occurred during async invocation.
 *
 */
class AsyncException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_EXCEPTIONS_H
