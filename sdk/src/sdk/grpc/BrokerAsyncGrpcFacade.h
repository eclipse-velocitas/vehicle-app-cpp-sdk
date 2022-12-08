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

#ifndef VEHICLE_APP_SDK_BROKERASYNCGRPCFACADE_H
#define VEHICLE_APP_SDK_BROKERASYNCGRPCFACADE_H

#include "sdk/grpc/AsyncGrpcFacade.h"
#include "sdk/grpc/GrpcClient.h"

#include "sdv/databroker/v1/broker.grpc.pb.h"

#include <memory>

namespace grpc {
class Channel;
}

namespace velocitas {

class BrokerAsyncGrpcFacade : public AsyncGrpcFacade, GrpcClient {
public:
    explicit BrokerAsyncGrpcFacade(std::shared_ptr<grpc::Channel> channel);

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
    std::shared_ptr<sdv::databroker::v1::Broker::Stub> m_stub;
    std::shared_ptr<grpc::Channel>                     m_channel;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_BROKERASYNCGRPCFACADE_H
