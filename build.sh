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

#
# Builds the targets of the project in different flavors.
#

set -e
source ./.scripts/common.sh

function print_help() {
  echo "Build targets of the project
============================================================================
Builds the targets of the project in different flavors.

Arguments:
-d, --debug                      Builds the target(s) in debug mode.
-r, --release                    Builds the target(s) in release mode.
-t <name>, --target <name>       Builds only the target <name> instead of all targets.
-no-examples                     Disables the build of the SDK examples.
-no-tests                        Disables the build of the SDK tests.
--cov                            Generates coverage information.
-s, --static                     Links all dependencies statically.
-x, --cross <arch>               Cross compiles for the specified architecture.
-h, --help                       Shows this help.
"
}

BUILD_TYPE=Debug
BUILD_ARCH=$(arch)
HOST_OS=linux
HOST_ARCH=${BUILD_ARCH}
BUILD_TARGET=all
STATIC_BUILD=OFF
SDK_BUILD_EXAMPLES=ON
SDK_BUILD_TESTS=ON
GEN_COVERAGE=OFF

POSITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
  case $1 in
    -d|--debug)
      BUILD_TYPE="Debug"
      shift
      ;;
    -r|--release)
      BUILD_TYPE="Release"
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
    --no-tests)
      SDK_BUILD_TESTS=OFF
      shift
      ;;
    -x|--cross)
      HOST_ARCH=$( get_valid_cross_compile_architecture "$2" )
      shift
      shift
      ;;
    --cov)
      GEN_COVERAGE=ON
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
echo "Build type         ${BUILD_TYPE}"
echo "Build arch         ${BUILD_ARCH}"
echo "Host arch          ${HOST_ARCH}"
echo "Build target       ${BUILD_TARGET}"
echo "Build SDK tests    ${SDK_BUILD_TESTS}"
echo "Build SDK examples ${SDK_BUILD_EXAMPLES}"
echo "Static build       ${STATIC_BUILD}"
echo "Coverage           ${GEN_COVERAGE}"

CMAKE_CXX_FLAGS="-g -O0"

if [ "${BUILD_TYPE}" == "Release" ]; then
    CMAKE_CXX_FLAGS="-s -g -O3"
fi

if [ "${GEN_COVERAGE}" == "ON" ]; then
  CMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} --coverage"
fi

BUILD_FOLDER=build-${HOST_OS}-${HOST_ARCH}/${BUILD_TYPE}
echo "Using build foler ${BUILD_FOLDER}"
# Set a symlink called 'build' to the last used build-type (release/debug) for
# a local build (i.e. host arch == build arch). This is required for  easy
# access via vscod's task and launch config and for other build and test tools
if [[ "${BUILD_ARCH}" == "${HOST_ARCH}" ]]; then
  echo "Setting/updating symlink 'build' to ${BUILD_FOLDER}"
  ln -snf ${BUILD_FOLDER} build
fi

SRC_FOLDER=$(pwd)
mkdir -p ${BUILD_FOLDER} && pushd ${BUILD_FOLDER}
source generators/conanbuild.sh

# Configure CMake and build the project.
cmake --no-warn-unused-cli \
  -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
  -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE} \
  -DSTATIC_BUILD:BOOL=${STATIC_BUILD} \
  -DSDK_BUILD_EXAMPLES=${SDK_BUILD_EXAMPLES} \
  -DSDK_BUILD_TESTS=${SDK_BUILD_TESTS} \
  -DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS}" \
  -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake \
  -G Ninja \
  -S ${SRC_FOLDER} \
  -B .
cmake --build . --target ${BUILD_TARGET}

popd >> /dev/null
