/**
 * Copyright (c) 2022-2024 Contributors to the Eclipse Foundation
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

#ifndef VMDL_EXAMPLE_VEHICLE_CABIN_SEATSERVICE_H
#define VMDL_EXAMPLE_VEHICLE_CABIN_SEATSERVICE_H

#include "sdk/AsyncResult.h"
#include "sdk/Model.h"

#include <memory>

namespace velocitas {
class SeatServiceAsyncGrpcFacade;
}

namespace velocitas::vehicle::cabin {

/**
 * @brief
 * Seats service for getting and controlling the positions of the seats
 * and their components in the vehicle.
 * This definition corresponds to the COVESA Vehicle Service Catalog (VSC)
 * comfort seats service definition
 * (https://github.com/COVESA/vehicle_service_catalog)
 * ...
 * Methods
 * ----------
 * move(sdv::edge::comfort::seats::v1::Seat* seat)
 *     Set the desired seat position
 * moveComponent(sdv::edge::comfort::seats::v1::SeatLocation* seatLocation,
 * sdv::edge::comfort::seats::v1::SeatComponent component, int position) Set a seat component
 * position getCurrentPosition(int row, int index) Get the current position of the addressed seat
 */

class SeatService : public velocitas::Service {
public:
    enum Component {
        Base,
        Cushion,
        Lumbar,
        SideBolster,
        HeadRestraint,
    };

    struct Positions {
        int base;          // The position of the base 0 front, 1000 back
        int cushion;       // The position of the cushion 0 short, 1000 long
        int lumbar;        // The position of the lumbar support
        int sideBolster;   // The position of the side bolster
        int headRestraint; // The position of the head restraint 0 down, 1000 up
    };

    struct SeatLocation {
        int row;
        int index;
    };

    struct Seat {
        SeatLocation location;
        Positions    position;
    };

    /**
     * @brief Construct a new Seat Service object
     *
     * @param parent The parent node in the model
     */
    explicit SeatService(Model* parent);

    /**
     * @brief
     *
     * @param seat
     * @return AsyncResultPtr_t<VoidResult>
     *
     *      Ok - Seat movement started
     *      OutOfRange - The addressed seat is not present in this vehicle
     *      InvalidArgument - At least one of the requested component positions is invalid
     *      Internal - A seat service internal error happened - see error message for details
     */
    AsyncResultPtr_t<VoidResult> move(const Seat& seat);

    /**
     * @brief
     *
     * @param seatLocation
     * @param component
     * @param position
     * @return AsyncResultPtr_t<VoidResult>
     *      Ok - Seat movement started
     *      OutOfRange - The addressed seat is not present in this vehicle
     *      NotFound - The addressed seat component is not supported by this seat/vehicle
     *      InvalidArgument - At least one of the requested component positions is invalid
     *      Internal - A seat service internal error happened - see error message for details
     */
    AsyncResultPtr_t<VoidResult> moveComponent(const SeatLocation& seatLocation,
                                               Component component, int position);

    /**
     * @brief Get the Current Position object
     *
     * @param row
     * @param index
     * @return A string containing the JSON response of the request. Returns an empty string if the
     * addressed seat is not present in this vehicle.
     */
    AsyncResultPtr_t<std::string> getCurrentPosition(int row, int index);

private:
    std::shared_ptr<SeatServiceAsyncGrpcFacade> m_asyncGrpcFacade;
};
} // namespace velocitas::vehicle::cabin
#endif // VMDL_EXAMPLE_VEHICLE_CABIN_SEATSERVICE_H
