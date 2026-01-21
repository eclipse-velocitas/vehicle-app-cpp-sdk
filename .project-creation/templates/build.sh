#!/bin/bash
# Copyright (c) 2023 Contributors to the Eclipse Foundation
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

#
# Builds the targets of the project in different flavors.
#

set -e

function print_help() {
  echo "Build targets of the project
============================================================================
Builds the targets of the project in different flavors.

Arguments:
-d, --debug                      Builds the target(s) in debug mode.
-r, --release                    Builds the target(s) in release mode.
-t <name>, --target <name>       Builds only the target <name> instead of all targets. 
-s, --static                     Links all dependencies statically. 
-h, --help                       Shows this help.
"
}

BUILD_VARIANT=debug
BUILD_ARCH=$(arch)
BUILD_TARGET=all
STATIC_BUILD=OFF

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
    -h|--help)
      print_help
      exit 0
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

echo "CMake version      "`cmake --version`
echo "Conan version      "`conan --version`
echo "Build variant      ${BUILD_VARIANT}"
echo "Build arch         ${BUILD_ARCH}"
echo "Build target       ${BUILD_TARGET}"
echo "Static build       ${STATIC_BUILD}"

CMAKE_CXX_FLAGS="--coverage -g -O0"

if [ "${BUILD_VARIANT}" == "release" ]; then
    CMAKE_CXX_FLAGS="--coverage -s -g -O3"
fi

mkdir -p build && cd build

# Expose the PATH of the build-time requirements from Conan to CMake - this is NOT handled by
# any of Conan's CMake generators at the moment, hence we parse the conanbuildinfo.txt which
# is generated and holds these paths. This allows us to always use the protoc and grpc cpp plugin
# of the build system.
BUILD_TOOLS_PATH=""
CONAN_BUILD_TOOLS_PATHS=$(sed '/^PATH=/!d;s/PATH=//g;s/,/\n/g' ./conanbuildinfo.txt | tr -d '[]'\" )
while read -r p; do
  if [[ ! -z "${p// }" ]]; then
    BUILD_TOOLS_PATH="$BUILD_TOOLS_PATH;$p"
  fi
done < <(echo "$CONAN_BUILD_TOOLS_PATHS")

cmake --no-warn-unused-cli \
     -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
     -DCMAKE_BUILD_TYPE:STRING=${BUILD_VARIANT} \
     -DBUILD_TOOLS_PATH:STRING="${BUILD_TOOLS_PATH}" \
     -DSTATIC_BUILD:BOOL=${STATIC_BUILD} \
     -S.. \
     -B../build \
     -G Ninja \
     -DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS}"

cmake --build . --config ${BUILD_VARIANT} --target ${BUILD_TARGET} -- 
cd ..
