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
echo "### Running HVAC Service from VAL                   ###"
echo "#######################################################"

ROOT_DIRECTORY=$( realpath "$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )/../.." )
source $ROOT_DIRECTORY/.vscode/scripts/exec-check.sh "$@" $(basename $BASH_SOURCE .sh)

SERVICE_VERSION=$(cat $ROOT_DIRECTORY/prerequisite_settings.json | jq .hvacservice.version | tr -d '"')
SERVICE_PORT='50052'
SERVICE_GRPC_PORT='52005'
HVACSERVICE_DAPR_APP_ID='hvacservice'

VEHICLEDATABROKER_DAPR_APP_ID='vehicledatabroker'
sudo chown $(whoami) $HOME

# Downloading Service Source
SERVICE_SOURCE="hvacservice_source"
SERVICE_EXEC_PATH="$ROOT_DIRECTORY/.vscode/scripts/assets/hvacservice/$SERVICE_VERSION"

API_URL=https://github.com/eclipse/kuksa.val.services/tarball

if [[ ! -f "$SERVICE_EXEC_PATH/hvac_service/hvacservice.py" ]]
then
  echo "Downloading Service Sources:$SERVICE_VERSION"
  curl --create-dirs -o "$ROOT_DIRECTORY/.vscode/scripts/assets/hvacservice/$SERVICE_VERSION/$SERVICE_SOURCE" --location --remote-header-name --remote-name "$API_URL/$SERVICE_VERSION"
  SERVICE_BASE_DIRECTORY=$(tar -tzf $ROOT_DIRECTORY/.vscode/scripts/assets/hvacservice/$SERVICE_VERSION/$SERVICE_SOURCE | head -1 | cut -f1 -d"/")
  tar -xf $ROOT_DIRECTORY/.vscode/scripts/assets/hvacservice/$SERVICE_VERSION/$SERVICE_SOURCE -C $ROOT_DIRECTORY/.vscode/scripts/assets/hvacservice/$SERVICE_VERSION/
  cp -r $ROOT_DIRECTORY/.vscode/scripts/assets/hvacservice/$SERVICE_VERSION/$SERVICE_BASE_DIRECTORY/hvac_service $ROOT_DIRECTORY/.vscode/scripts/assets/hvacservice/$SERVICE_VERSION
  rm -rf $ROOT_DIRECTORY/.vscode/scripts/assets/hvacservice/$SERVICE_VERSION/$SERVICE_BASE_DIRECTORY
fi
cd $ROOT_DIRECTORY/.vscode/scripts/assets/hvacservice/$SERVICE_VERSION/hvac_service
pip3 install -r requirements.txt

export DAPR_GRPC_PORT=$SERVICE_GRPC_PORT
export HVACSERVICE_DAPR_APP_ID=$HVACSERVICE_DAPR_APP_ID
export VEHICLEDATABROKER_DAPR_APP_ID=$VEHICLEDATABROKER_DAPR_APP_ID

dapr run \
  --app-id hvacservice \
  --app-protocol grpc \
  --app-port $SERVICE_PORT \
  --dapr-grpc-port $SERVICE_GRPC_PORT \
  --components-path $ROOT_DIRECTORY/.dapr/components \
  --config $ROOT_DIRECTORY/.dapr/config.yaml -- python3 hvacservice.py
