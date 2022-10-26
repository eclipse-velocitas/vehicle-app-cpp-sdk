#!/bin/bash
# Copyright (c) 2022 Robert Bosch GmbH and Microsoft Corporation
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

if [[ "$#" -eq 1 ]]; then
    tput setaf 1; echo "ERROR: To execute script, use VSCODE Tasks: [CTRL+SHIFT+P -> Tasks: Run Tasks -> $1]."
    read -p "Press <Enter> to close this window"
    exit 1
fi
