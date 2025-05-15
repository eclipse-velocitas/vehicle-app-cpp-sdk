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

#ifndef VEHICLE_APP_SDK_BROKERASYNCGRPCFACADE_H
#define VEHICLE_APP_SDK_BROKERASYNCGRPCFACADE_H

#include "sdk/grpc/AsyncGrpcFacade.h"
#include "sdk/grpc/GrpcClient.h"

#include "sdv/databroker/v1/broker.grpc.pb.h"

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace grpc {
class Channel;
class Status;
} // namespace grpc

namespace velocitas::sdv_databroker_v1 {

class BrokerAsyncGrpcFacade : public AsyncGrpcFacade, GrpcClient {
public:
    explicit BrokerAsyncGrpcFacade(const std::shared_ptr<grpc::Channel>& channel);

    void GetDatapoints(
        const std::vector<std::string>&                                           datapoints,
        std::function<void(const sdv::databroker::v1::GetDatapointsReply& reply)> replyHandler,
        std::function<void(const grpc::Status& status)>                           errorHandler);

    void SetDatapoints(
        const std::map<std::string, sdv::databroker::v1::Datapoint>&              datapoints,
        std::function<void(const sdv::databroker::v1::SetDatapointsReply& reply)> replyHandler,
        std::function<void(const grpc::Status& status)>                           errorHandler);

    void
    Subscribe(const std::string&                                                    query,
              std::function<void(const sdv::databroker::v1::SubscribeReply& reply)> itemHandler,
              std::function<void(const grpc::Status& status)>                       errorHandler);

private:
    std::unique_ptr<sdv::databroker::v1::Broker::StubInterface> m_stub;
};

} // namespace velocitas::sdv_databroker_v1

#endif // VEHICLE_APP_SDK_BROKERASYNCGRPCFACADE_H
