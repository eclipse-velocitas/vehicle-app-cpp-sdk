#!/bin/bash
# Copyright (c) 2022-2024 Contributors to the Eclipse Foundation
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
echo "### Running VehicleServices                         ###"
echo "#######################################################"

ROOT_DIRECTORY=$( realpath "$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )/../../../.." )

# Get Data from AppManifest.json and save to ENV
UTILS_DIRECTORY="$ROOT_DIRECTORY/.vscode/scripts/runtime/utils"
source $UTILS_DIRECTORY/get-appmanifest-data.sh

# Configure Service Specific Requirements
configure_service() {
    case $1 in
        seatservice)
            export SERVICE_PORT=50051
            export CAN=cansim
            export VEHICLEDATABROKER_DAPR_APP_ID=vehicledatabroker
            # Configure docker networking, e.g. ports for docker to expose
            DOCKER_NET_CONFIG="--network host"
            # Configure ENVs need to run docker container
            DOCKER_ENVS="-e VEHICLEDATABROKER_DAPR_APP_ID -e CAN -e DAPR_GRPC_PORT -e DAPR_HTTP_PORT -e SERVICE_PORT"
            ;;
        *)
            echo "Unknown Service to configure."
            ;;
    esac
}

# Run Docker Container with Dapr Sidecar of configured service
run_service() {
    configure_service $1

    RUNNING_CONTAINER=$(docker ps | grep "$SERVICE_IMAGE" | awk '{ print $1 }')

    if [ -n "$RUNNING_CONTAINER" ];
    then
        docker container stop $RUNNING_CONTAINER
    fi

    dapr run \
        --app-id $SERVICE_NAME \
        --app-protocol grpc \
        --app-port $SERVICE_PORT \
        --components-path $ROOT_DIRECTORY/.dapr/components \
        --config $ROOT_DIRECTORY/.dapr/config.yaml \
    -- docker run $DOCKER_NET_CONFIG $DOCKER_ENVS $SERVICE_IMAGE:$SERVICE_TAG &
}

DEPENDENCIES=$(cat $ROOT_DIRECTORY/AppManifest.json | jq .[].dependencies)
SERVICES=$(echo $DEPENDENCIES | jq '.services')

if [ "$SERVICES" = "null" ];then
    echo "No Services defined in AppManifest. Skip running vehicle services.";
else
    readarray -t SERVICES_ARRAY < <(echo $SERVICES | jq -c '.[]')

    for service in ${SERVICES_ARRAY[@]}; do
        SERVICE_NAME=$(echo $service | jq '.name' | tr -d '"' )
        SERVICE_IMAGE=$(echo $service | jq '.image' | tr -d '"')
        SERVICE_TAG=$(echo $service | jq '.version' | tr -d '"')
        if [ $SERVICE_IMAGE = "null" ] || [ $SERVICE_TAG = "null" ];then
            echo "Missing configuration in AppManifest.json for Service: $SERVICE_NAME"
        else
            echo "Starting Service: $SERVICE_NAME"
            run_service $SERVICE_NAME
        fi
    done
fi

wait
