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

#ifndef VMDL_EXAMPLE_VEHICLE_CABIN_H
#define VMDL_EXAMPLE_VEHICLE_CABIN_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle_model/Cabin/Seat/Seat.h"
#include "vehicle_model/Cabin/SeatService/SeatService.h"

#include <stdexcept>

namespace velocitas::vehicle {
/** Cabin model. */
class Cabin : public Model {
public:
    class SeatCollection {
    public:
        class Row {
        public:
            explicit Row(int value) {
                if ((value < 1) || (value > 2)) {
                    throw std::runtime_error("Given value is outside of allowed range!");
                }
                m_value = value;
            }

            [[nodiscard]] int getValue() const { return m_value; }

        private:
            int m_value;
        };

        class Pos {
        public:
            explicit Pos(int value) {
                if ((value < 1) || (value > 3)) {
                    throw std::runtime_error("Given value is outside of allowed range!");
                }
                m_value = value;
            }

            [[nodiscard]] int getValue() const { return m_value; }

        private:
            int m_value;
        };

        explicit SeatCollection(Model* parent)
            : m_row1Pos1("Seat.Row1.Pos1", parent)
            , m_row1Pos2("Seat.Row1.Pos2", parent)
            , m_row1Pos3("Seat.Row1.Pos3", parent)
            , m_row2Pos1("Seat.Row2.Pos1", parent)
            , m_row2Pos2("Seat.Row2.Pos2", parent)
            , m_row2Pos3("Seat.Row2.Pos3", parent) {}

        vehicle::cabin::Seat& elementAt(Row p0, Pos p1) {
            if ((p0.getValue() == 1) && (p1.getValue() == 1)) {
                return m_row1Pos1;
            }
            if ((p0.getValue() == 1) && (p1.getValue() == 2)) {
                return m_row1Pos2;
            }
            if ((p0.getValue() == 1) && (p1.getValue() == 3)) {
                return m_row1Pos3;
            }
            if ((p0.getValue() == 2) && (p1.getValue() == 1)) {
                return m_row2Pos1;
            }
            if ((p0.getValue() == 2) && (p1.getValue() == 2)) {
                return m_row2Pos2;
            }
            if ((p0.getValue() == 2) && (p1.getValue() == 3)) {
                return m_row2Pos3;
            }
            throw std::runtime_error("Invalid parameters!");
        }

    private:
        vehicle::cabin::Seat m_row1Pos1;
        vehicle::cabin::Seat m_row1Pos2;
        vehicle::cabin::Seat m_row1Pos3;
        vehicle::cabin::Seat m_row2Pos1;
        vehicle::cabin::Seat m_row2Pos2;
        vehicle::cabin::Seat m_row2Pos3;
    };

    Cabin(const std::string& name, Model* parent);

    /**
     * Seat: branch
     * All seats.
     *
     **/
    SeatCollection& getSeat() { return m_seat; }

    vehicle::cabin::SeatService& getSeatService() { return m_seatService; }

private:
    SeatCollection              m_seat;
    vehicle::cabin::SeatService m_seatService;
};
} // namespace velocitas::vehicle
#endif // VMDL_EXAMPLE_VEHICLE_CABIN_H
