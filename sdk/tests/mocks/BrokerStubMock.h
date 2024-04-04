/**
 * Copyright (c) 2024 Contributors to the Eclipse Foundation
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

#ifndef VEHICLE_APP_SDK_BROKERSTUBMOCK_H
#define VEHICLE_APP_SDK_BROKERSTUBMOCK_H

#include "sdv/databroker/v1/broker.grpc.pb.h"

#include <gmock/gmock.h>

namespace velocitas {

class BrokerStubMock : public sdv::databroker::v1::Broker::StubInterface,
                       public sdv::databroker::v1::Broker::StubInterface::async_interface {
public:
    MOCK_METHOD(void, GetDatapoints,
                (::grpc::ClientContext * context,
                 const ::sdv::databroker::v1::GetDatapointsRequest* request,
                 ::sdv::databroker::v1::GetDatapointsReply*         response,
                 std::function<void(::grpc::Status)>));
    MOCK_METHOD(void, SetDatapoints,
                (::grpc::ClientContext * context,
                 const ::sdv::databroker::v1::SetDatapointsRequest* request,
                 ::sdv::databroker::v1::SetDatapointsReply*         response,
                 std::function<void(::grpc::Status)>));
    MOCK_METHOD(void, Subscribe,
                (::grpc::ClientContext * context,
                 const ::sdv::databroker::v1::SubscribeRequest*                    request,
                 ::grpc::ClientReadReactor<::sdv::databroker::v1::SubscribeReply>* reactor));
    MOCK_METHOD(void, GetMetadata,
                (::grpc::ClientContext * context,
                 const ::sdv::databroker::v1::GetMetadataRequest* request,
                 ::sdv::databroker::v1::GetMetadataReply*         response,
                 std::function<void(::grpc::Status)>));

    class sdv::databroker::v1::Broker::StubInterface::async_interface* async() override {
        return this;
    }

    void GetDatapoints(::grpc::ClientContext*                             context,
                       const ::sdv::databroker::v1::GetDatapointsRequest* request,
                       ::sdv::databroker::v1::GetDatapointsReply*         response,
                       ::grpc::ClientUnaryReactor*                        reactor) override {}
    void SetDatapoints(::grpc::ClientContext*                             context,
                       const ::sdv::databroker::v1::SetDatapointsRequest* request,
                       ::sdv::databroker::v1::SetDatapointsReply*         response,
                       ::grpc::ClientUnaryReactor*                        reactor) override {}
    void GetMetadata(::grpc::ClientContext*                           context,
                     const ::sdv::databroker::v1::GetMetadataRequest* request,
                     ::sdv::databroker::v1::GetMetadataReply*         response,
                     ::grpc::ClientUnaryReactor*                      reactor) override {}

    ::grpc::Status GetDatapoints(::grpc::ClientContext*                             context,
                                 const ::sdv::databroker::v1::GetDatapointsRequest& request,
                                 ::sdv::databroker::v1::GetDatapointsReply* response) override {
        return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    ::grpc::Status SetDatapoints(::grpc::ClientContext*                             context,
                                 const ::sdv::databroker::v1::SetDatapointsRequest& request,
                                 ::sdv::databroker::v1::SetDatapointsReply* response) override {
        return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    ::grpc::Status GetMetadata(::grpc::ClientContext*                           context,
                               const ::sdv::databroker::v1::GetMetadataRequest& request,
                               ::sdv::databroker::v1::GetMetadataReply*         response) override {
        return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }

    ::grpc::ClientAsyncResponseReaderInterface<::sdv::databroker::v1::GetDatapointsReply>*
    AsyncGetDatapointsRaw(::grpc::ClientContext*                             context,
                          const ::sdv::databroker::v1::GetDatapointsRequest& request,
                          ::grpc::CompletionQueue*                           cq) override {
        return nullptr;
    }
    ::grpc::ClientAsyncResponseReaderInterface<::sdv::databroker::v1::GetDatapointsReply>*
    PrepareAsyncGetDatapointsRaw(::grpc::ClientContext*                             context,
                                 const ::sdv::databroker::v1::GetDatapointsRequest& request,
                                 ::grpc::CompletionQueue*                           cq) override {
        return nullptr;
    }
    ::grpc::ClientAsyncResponseReaderInterface<::sdv::databroker::v1::SetDatapointsReply>*
    AsyncSetDatapointsRaw(::grpc::ClientContext*                             context,
                          const ::sdv::databroker::v1::SetDatapointsRequest& request,
                          ::grpc::CompletionQueue*                           cq) override {
        return nullptr;
    }
    ::grpc::ClientAsyncResponseReaderInterface<::sdv::databroker::v1::SetDatapointsReply>*
    PrepareAsyncSetDatapointsRaw(::grpc::ClientContext*                             context,
                                 const ::sdv::databroker::v1::SetDatapointsRequest& request,
                                 ::grpc::CompletionQueue*                           cq) override {
        return nullptr;
    }
    ::grpc::ClientReaderInterface<::sdv::databroker::v1::SubscribeReply>*
    SubscribeRaw(::grpc::ClientContext*                         context,
                 const ::sdv::databroker::v1::SubscribeRequest& request) override {
        return nullptr;
    }
    ::grpc::ClientAsyncReaderInterface<::sdv::databroker::v1::SubscribeReply>*
    AsyncSubscribeRaw(::grpc::ClientContext*                         context,
                      const ::sdv::databroker::v1::SubscribeRequest& request,
                      ::grpc::CompletionQueue* cq, void* tag) override {
        return nullptr;
    }
    ::grpc::ClientAsyncReaderInterface<::sdv::databroker::v1::SubscribeReply>*
    PrepareAsyncSubscribeRaw(::grpc::ClientContext*                         context,
                             const ::sdv::databroker::v1::SubscribeRequest& request,
                             ::grpc::CompletionQueue*                       cq) override {
        return nullptr;
    }
    ::grpc::ClientAsyncResponseReaderInterface<::sdv::databroker::v1::GetMetadataReply>*
    AsyncGetMetadataRaw(::grpc::ClientContext*                           context,
                        const ::sdv::databroker::v1::GetMetadataRequest& request,
                        ::grpc::CompletionQueue*                         cq) override {
        return nullptr;
    }
    ::grpc::ClientAsyncResponseReaderInterface<::sdv::databroker::v1::GetMetadataReply>*
    PrepareAsyncGetMetadataRaw(::grpc::ClientContext*                           context,
                               const ::sdv::databroker::v1::GetMetadataRequest& request,
                               ::grpc::CompletionQueue*                         cq) override {
        return nullptr;
    }
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_BROKERSTUBMOCK_H
