#!/bin/bash
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

set -e

BUILD_VARIANT=debug
BUILD_ARCH=$(arch)
BUILD_TARGET=all
STATIC_BUILD=OFF
SDK_BUILD_EXAMPLES=ON

POSITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
  case $1 in
    -d|--debug)
      BUILD_VARIANT="debug"
      shift
      ;;
    -r|--release)
      BUILD_VARIANT="release"
      shift
      ;;
    -t|--target)
      BUILD_TARGET="$2"
      shift
      shift
      ;;
    -s|--static)
      STATIC_BUILD=ON
      shift
      ;;
    --no-examples)
      SDK_BUILD_EXAMPLES=OFF
      shift
      ;;
    -*|--*)
      echo "Unknown option $1"
      exit 1
      ;;
    *)
      POSITIONAL_ARGS+=("$1")
      shift
      ;;
  esac
done

echo "Build variant      ${BUILD_VARIANT}"
echo "Build arch         ${BUILD_ARCH}"
echo "Build target       ${BUILD_TARGET}"
echo "Build SDK examples ${SDK_BUILD_EXAMPLES}"
echo "Static build       ${STATIC_BUILD}"

CMAKE_CXX_FLAGS="--coverage -g -O0"

if [ "${BUILD_VARIANT}" == "release" ]; then
    CMAKE_CXX_FLAGS="--coverage -s -g -O3"
fi

mkdir -p build && cd build
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=${BUILD_VARIANT} -DSTATIC_BUILD:BOOL=${STATIC_BUILD} -DSDK_BUILD_EXAMPLES=${SDK_BUILD_EXAMPLES} -S.. -B../build -G Ninja -DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS}"
cmake --build . --config ${BUILD_VARIANT} --target ${BUILD_TARGET} -- 
cd ..
