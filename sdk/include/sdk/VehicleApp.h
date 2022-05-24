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

#ifndef VEHICLE_APP_SDK_VEHICLEAPP_H
#define VEHICLE_APP_SDK_VEHICLEAPP_H

#include "sdk/AsyncResult.h"
#include "sdk/DataPoint.h"
#include "sdk/DataPointsResult.h"

#include <functional>
#include <memory>
#include <string>

namespace velocitas {

class IVehicleDataBrokerClient;
class IPubSubClient;

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
     * @param pubSubClient  The pubsub client to use.
     */
    explicit VehicleApp(std::shared_ptr<IVehicleDataBrokerClient> vdbClient,
                        std::shared_ptr<IPubSubClient>            pubSubClient);

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
     * @return AsyncResultPtr_t<DataPointsResult>  The result that contains data point results for
     * all requested data points.
     */
    AsyncResultPtr_t<DataPointsResult>
    getDataPoints(const std::vector<std::reference_wrapper<DataPoint>>& dataPoints);

    /**
     * @brief Subscribes to the query string for data points.
     *
     * @param queryString   The query string to subscribe to.
     * @return AsyncSubscriptionPtr_t<DataPointsResult>   The subscription to the data points.
     */
    AsyncSubscriptionPtr_t<DataPointsResult> subscribeDataPoints(const std::string& queryString);

    /**
     * @brief Get the Vehicle Data Broker Client object.
     *
     * @return std::shared_ptr<IVehicleDataBrokerClient>
     */
    std::shared_ptr<IVehicleDataBrokerClient> getVehicleDataBrokerClient();

    /**
     * @brief Get the Pub Sub Client object
     *
     * @return std::shared_ptr<IPubSubClient>
     */
    std::shared_ptr<IPubSubClient> getPubSubClient();

private:
    std::shared_ptr<IVehicleDataBrokerClient> m_vdbClient;
    std::shared_ptr<IPubSubClient>            m_pubSubClient;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_VEHICLEAPP_H
