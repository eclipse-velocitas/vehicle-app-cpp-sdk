#!/bin/bash
# Copyright (c) 2022-2025 Contributors to the Eclipse Foundation
#
# This program and the accompanying materials are made available under the
# terms of the Apache License, Version 2.0 which is available at
# https://www.apache.org/licenses/LICENSE-2.0.
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.
#
# SPDX-License-Identifier: Apache-2.0

function get_valid_cross_compile_architecture() {
    if [[ "$1" == "aarch64" || "$1" == "arm64" || "$1" == "armv8" ]]; then
      HOST_ARCH="aarch64"
    elif [[ "$1" == "x86_64" || "$1" == "amd64" ]]; then
      HOST_ARCH="x86_64"
    else
      return 1
    fi

    echo $HOST_ARCH
    return 0
}
