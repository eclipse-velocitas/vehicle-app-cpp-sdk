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

#ifndef VEHICLE_APP_SDK_VEHICLEAPP_H
#define VEHICLE_APP_SDK_VEHICLEAPP_H

#include "sdk/AsyncResult.h"
#include "sdk/DataPointReply.h"

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>

namespace velocitas {

class DataPoint;
class IPubSubClient;
class IVehicleDataBrokerClient;

/**
 * @brief Base class for all vehicle apps which manages an app's lifecycle.
 *
 */
class VehicleApp {
public:
    /**
     * @brief Construct a new Vehicle App object.
     *
     * @param vdbClient     The vehicle data broker client to use.
     * @param pubSubClient  The pubsub client to use. Using a pubsub client is optional.
     *                      If passing a nullptr, the app will not use/offer pubsub.
     */
    explicit VehicleApp(std::shared_ptr<IVehicleDataBrokerClient> vdbClient,
                        std::shared_ptr<IPubSubClient>            pubSubClient = {});

    virtual ~VehicleApp() = default;

    /**
     * @brief Runs the Vehicle App.
     *
     */
    void run();

    /**
     * @brief Stops the Vehicle App
     *
     */
    void stop();

    /**
     * @brief Event which is called once the Vehicle App is started.
     *
     */
    virtual void onStart() {}

    /**
     * @brief Event which is called once the Vehicle App is requested to stop.
     *
     */
    virtual void onStop() {}

    VehicleApp(const VehicleApp&)            = delete;
    VehicleApp(VehicleApp&&)                 = delete;
    VehicleApp& operator=(const VehicleApp&) = delete;
    VehicleApp& operator=(VehicleApp&&)      = delete;

protected:
    /**
     * @brief Subscribes to the given PubSub topic.
     *
     * @param topic   The topic to subscribe to.
     * @return AsyncSubscriptionPtr_t<std::string>  The subscription to the topic.
     */
    AsyncSubscriptionPtr_t<std::string> subscribeToTopic(const std::string& topic);

    /**
     * @brief Publish a PubSub message to the given topic.
     *
     * @param topic   The topic to publish to.
     * @param data    The message data to publish in JSON format.
     */
    void publishToTopic(const std::string& topic, const std::string& data);

    /**
     * @brief Get values for all provided data points from the data broker.
     *
     * @param dataPoints    Vector of data points to obtain values for.
     * @return The reply containing the data point values for all requested data points.
     */
    AsyncResultPtr_t<DataPointReply>
    getDataPoints(const std::vector<std::reference_wrapper<DataPoint>>& dataPoints);

    /**
     * @brief Get the value a certain data point from the data broker.
     *
     * @param dataPoint    The data point to obtain values for.
     * @return AsyncResultPtr_t<typename TDataPoint::value_type>  The result containing
     *     the data point value of the requested data point.
     */
    template <typename TDataPoint>
    [[nodiscard]] AsyncResultPtr_t<typename TDataPoint::value_type>
    getDataPoint(const TDataPoint& dataPoint) const {
        return getDataPoint_internal(dataPoint)->template map<typename TDataPoint::value_type>(
            [&dataPoint](const DataPointReply& dataPointValues) {
                return dataPointValues.get<TDataPoint>(dataPoint)->value();
            });
    }

    /**
     * @brief Subscribes to the query for data points.
     *
     * @param queryString   The query to subscribe to.
     * @return The subscription to the data points.
     */
    AsyncSubscriptionPtr_t<DataPointReply> subscribeDataPoints(const std::string& queryString);

    /**
     * @brief Get the Vehicle Data Broker Client object.
     *
     * @return std::shared_ptr<IVehicleDataBrokerClient>
     */
    std::shared_ptr<IVehicleDataBrokerClient> getVehicleDataBrokerClient();

    /**
     * @brief Get the Pub Sub Client object
     *
     * @return std::shared_ptr<IPubSubClient>; will be nullptr if no PubSubClient was instantiated
     */
    std::shared_ptr<IPubSubClient> getPubSubClient();

private:
    [[nodiscard]] AsyncResultPtr_t<DataPointReply>
    getDataPoint_internal(const DataPoint& dataPoint) const;

    std::shared_ptr<IVehicleDataBrokerClient> m_vdbClient;
    std::shared_ptr<IPubSubClient>            m_pubSubClient;
    bool                                      m_isRunning{false};
    std::mutex                                m_stopWaitMutex;
    std::condition_variable                   m_stopWaitCV;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_VEHICLEAPP_H
