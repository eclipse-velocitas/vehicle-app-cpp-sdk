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

echo "#######################################################"
echo "### Running VehicleDataBroker CLI                   ###"
echo "#######################################################"

ROOT_DIRECTORY=$( realpath "$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )/../../../.." )

# Get Data from AppManifest.json and save to ENV
UTILS_DIRECTORY="$ROOT_DIRECTORY/.vscode/scripts/runtime/utils"
source $UTILS_DIRECTORY/get-appmanifest-data.sh

sudo chown $(whoami) $HOME

DATABROKER_REPO="https://github.com/boschglobal/kuksa.val"
# Needed because of how the databroker release is tagged
DATABROKER_VERSION=$DATABROKER_TAG

#Detect host environment (distinguish for Mac M1 processor)
if [[ `uname -m` == 'aarch64' || `uname -m` == 'arm64' ]]; then
    PROCESSOR="aarch64"
else
    PROCESSOR="x86_64"
fi
echo "Detected ${PROCESSOR} architecture"
DATABROKER_BINARY_NAME="databroker_${PROCESSOR}.tar.gz"
DATABROKER_INSTALL_PATH="$ROOT_DIRECTORY/.vscode/scripts/assets/databroker/$DATABROKER_VERSION/$PROCESSOR"
DATABROKER_EXEC_PATH="$DATABROKER_INSTALL_PATH/target/release"

if [[ ! -f "$DATABROKER_EXEC_PATH/databroker" ]]
then
    DL_URL="${DATABROKER_REPO}/releases/download/${DATABROKER_VERSION}/${DATABROKER_BINARY_NAME}"
    echo "Downloading ${DATABROKER_BINARY_NAME} v$DATABROKER_VERSION"
    curl -o $DATABROKER_INSTALL_PATH/$DATABROKER_BINARY_NAME --create-dirs -L -H "Accept: application/octet-stream" "$DL_URL"
    tar -xf $DATABROKER_INSTALL_PATH/$DATABROKER_BINARY_NAME -C $DATABROKER_INSTALL_PATH
fi

$DATABROKER_EXEC_PATH/databroker-cli
