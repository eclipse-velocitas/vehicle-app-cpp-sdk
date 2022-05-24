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

#include "SeatAdjusterApp.h"
#include "sdk/Logger.h"

#include <csignal>
#include <fmt/core.h>

void handler(int sig) {
    velocitas::logger().info("Got signal {}, terminating.");
    exit(-1);
}

int main(int argc, char** argv) {
    signal(SIGINT, handler);

    example::SeatAdjusterApp myApp;
    myApp.run();
    return 0;
}
