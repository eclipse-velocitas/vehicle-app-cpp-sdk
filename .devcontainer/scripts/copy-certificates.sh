#!/bin/bash -e

# Copyright (c) 2022 Robert Bosch GmbH
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

if [ -n "${http_proxy}" ] || [ -n "${HTTP_PROXY}" ] || [ -n "${https_proxy}" ] || [ -n "${HTTPS_PROXY}" ]; then

    echo "Proxy configuration found - trying to copy certificates"

    if [ -d /usr/share/ca-certificates ]; then
        mkdir -p ./.devcontainer/ca-certificates
        cp -rfu /usr/share/ca-certificates/. -t ./.devcontainer/ca-certificates
    else
        echo No folder /usr/share/ca-certificates found.
    fi

    if [ -d /usr/local/share/ca-certificates ]; then
        mkdir -p ./.devcontainer/ca-certificates
        cp -rfu /usr/local/share/ca-certificates/. -t ./.devcontainer/ca-certificates
    else
        echo No folder /usr/local/share/ca-certificates found.
    fi
fi
