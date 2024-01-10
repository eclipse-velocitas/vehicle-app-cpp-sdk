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

#ifndef VMDL_EXAMPLE_VEHICLE_CABIN_H
#define VMDL_EXAMPLE_VEHICLE_CABIN_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle_model/Cabin/Seat/Seat.h"
#include "vehicle_model/Cabin/SeatService/SeatService.h"

#include <stdexcept>

namespace velocitas::vehicle {

using ParentClass = Model;

/** Cabin model. */
class Cabin : public ParentClass {
public:
    class SeatCollection : public ParentClass {
    public:
        class RowType : public ParentClass {
        public:
            RowType(std::string name, ParentClass* parent)
                : ParentClass(name, parent)
                , Pos1("Pos1", this)
                , Pos2("Pos2", this)
                , Pos3("Pos3", this) {}

            vehicle::cabin::Seat& Pos(int index) {
                if (index == 1) {
                    return Pos1;
                }
                if (index == 2) {
                    return Pos2;
                }
                if (index == 3) {
                    return Pos3;
                }
                throw std::runtime_error("Given value is outside of allowed range [1;3]!");
            }

            vehicle::cabin::Seat Pos1;
            vehicle::cabin::Seat Pos2;
            vehicle::cabin::Seat Pos3;
        };

        explicit SeatCollection(ParentClass* parent)
            : ParentClass("Seat", parent)
            , Row1("Row1", this)
            , Row2("Row2", this) {}

        RowType& Row(int index) {
            if (index == 1) {
                return Row1;
            }
            if (index == 2) {
                return Row2;
            }
            throw std::runtime_error("Given value is outside of allowed range [1;2]!");
        }

        RowType Row1;
        RowType Row2;
    };

    Cabin(const std::string& name, ParentClass* parent)
        : ParentClass(name, parent)
        , Seat(this)
        , SeatService(this) {}

    /**
     * Seat: branch
     * All seats.
     *
     **/
    SeatCollection              Seat;
    vehicle::cabin::SeatService SeatService;
};

} // namespace velocitas::vehicle

#endif // VMDL_EXAMPLE_VEHICLE_CABIN_H
