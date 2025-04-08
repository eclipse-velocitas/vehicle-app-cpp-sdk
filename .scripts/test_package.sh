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

HOST_PROFILE=$1
if [ "${HOST_PROFILE}" == "" ]; then
    HOST_PROFILE="linux-$(arch)"
    echo "Missing host profile argument - using default: ${HOST_PROFILE}"
fi

conan create --build=missing -pr:b ./.conan/profiles/linux-x86_64 -pr:h ./.conan/profiles/${HOST_PROFILE} -s:a="build_type=Release" . --user ci --channel testing