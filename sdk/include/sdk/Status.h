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

#ifndef VEHICLE_APP_SDK_STATUS_H
#define VEHICLE_APP_SDK_STATUS_H

#include <string>
#include <utility>

namespace velocitas {

/**
 * @brief Status of an asynchronous request.
 *
 */
class Status final {
public:
    /**
     * @brief Construct a new Status object without any errors.
     *
     */
    Status(){};

    /**
     * @brief Construct a new error status.
     *
     * @param errorMsg
     */
    Status(std::string errorMsg)
        : m_isOk(false)
        , m_errorMsg(std::move(errorMsg)) {}

    /**
     * @brief Returns whether the reported status is OK or not.
     *
     * @return true   Status is OK
     * @return false  Status is not OK
     */
    [[nodiscard]] bool ok() const { return m_isOk; }

    /**
     * @brief Return the error message stored within the status.
     *
     * @return const std::string& Error message. May be empty if status is ok.
     */
    [[nodiscard]] const std::string& errorMessage() const { return m_errorMsg; }

private:
    bool        m_isOk{true};
    std::string m_errorMsg;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_STATUS_H
