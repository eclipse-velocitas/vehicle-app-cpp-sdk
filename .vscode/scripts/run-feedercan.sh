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
echo "### Running FeederCan                               ###"
echo "#######################################################"

ROOT_DIRECTORY=$( realpath "$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )/../.." )
source $ROOT_DIRECTORY/.vscode/scripts/exec-check.sh "$@" $(basename $BASH_SOURCE .sh)

FEEDERCAN_VERSION=$(cat $ROOT_DIRECTORY/prerequisite_settings.json | jq .feedercan.version | tr -d '"')
DATABROKER_GRPC_PORT='52001'
sudo chown $(whoami) $HOME

# Downloading feedercan
FEEDERCAN_SOURCE="kuksa.val.feeders"
FEEDERCAN_EXEC_PATH="$ROOT_DIRECTORY/.vscode/scripts/assets/feedercan/$FEEDERCAN_VERSION"

DOWNLOAD_URL=https://github.com/eclipse/kuksa.val.feeders/tarball

if [[ ! -f "$FEEDERCAN_EXEC_PATH/dbc2val/dbcfeeder.py" ]]
then
  echo "Downloading FEEDERCAN:$FEEDERCAN_VERSION"
  curl --create-dirs -o "$ROOT_DIRECTORY/.vscode/scripts/assets/feedercan/$FEEDERCAN_VERSION/$FEEDERCAN_SOURCE" --location --remote-header-name --remote-name "$DOWNLOAD_URL/$FEEDERCAN_VERSION"
  FEEDERCAN_BASE_DIRECTORY=$(tar -tzf $ROOT_DIRECTORY/.vscode/scripts/assets/feedercan/$FEEDERCAN_VERSION/$FEEDERCAN_SOURCE | head -1 | cut -f1 -d"/")
  tar -xf $ROOT_DIRECTORY/.vscode/scripts/assets/feedercan/$FEEDERCAN_VERSION/$FEEDERCAN_SOURCE -C $ROOT_DIRECTORY/.vscode/scripts/assets/feedercan/$FEEDERCAN_VERSION/
  cp -r $ROOT_DIRECTORY/.vscode/scripts/assets/feedercan/$FEEDERCAN_VERSION/$FEEDERCAN_BASE_DIRECTORY/dbc2val $ROOT_DIRECTORY/.vscode/scripts/assets/feedercan/$FEEDERCAN_VERSION
  rm -rf $ROOT_DIRECTORY/.vscode/scripts/assets/feedercan/$FEEDERCAN_VERSION/$FEEDERCAN_BASE_DIRECTORY
fi
cd $ROOT_DIRECTORY/.vscode/scripts/assets/feedercan/$FEEDERCAN_VERSION/dbc2val
pip3 install -r requirements.txt

export DAPR_GRPC_PORT=$DATABROKER_GRPC_PORT
export VEHICLEDATABROKER_DAPR_APP_ID=vehicledatabroker
export LOG_LEVEL=info,databroker=info,dbcfeeder.broker_client=debug,dbcfeeder=debug

CONFIG_DIR="$ROOT_DIRECTORY/.vscode/scripts/feeder_config"
export USECASE="databroker"
if [ $1 == "DOGMODE" ]; then
  echo "Use DogMode feeder config ...!"
  # DogMode CAN feeder config
  export DBC_FILE="$CONFIG_DIR/dogmode/DogMode.dbc"
  export MAPPING_FILE="$CONFIG_DIR/dogmode/mapping_DogMode.yml"
  export CANDUMP_FILE="$CONFIG_DIR/dogmode/candump_DogMode.log"
else
  # Default CAN feeder config
  export DBC_FILE="$CONFIG_DIR/default/Model3CAN.dbc"
  export MAPPING_FILE="$CONFIG_DIR/default/mapping.yml"
  export CANDUMP_FILE="$CONFIG_DIR/default/candump.log"
fi

dapr run \
    --app-id feedercan \
    --app-protocol grpc \
    --components-path $ROOT_DIRECTORY/.dapr/components \
    --config $ROOT_DIRECTORY/.dapr/config.yaml -- python3 dbcfeeder.py
