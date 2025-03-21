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
HOST_OS=Linux
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
      HOST_ARCH="$2"
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

BUILD_FOLDER=build
XCOMPILE_TOOLCHAIN_FILE=""
if [[ "${BUILD_ARCH}" != "${HOST_ARCH}" ]]; then
  echo "Setting up cross compilation toolchain..."
  BUILD_FOLDER=build-${HOST_OS}-${HOST_ARCH}
  XCOMPILE_TOOLCHAIN_FILE="-DCMAKE_TOOLCHAIN_FILE=../cmake/${BUILD_ARCH}_to_${HOST_ARCH}.cmake"
fi
BUILD_FOLDER=${BUILD_FOLDER}/${BUILD_TYPE}

# Configure CMake and build the project.
cmake --no-warn-unused-cli \
  -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
  -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE} \
  -DSTATIC_BUILD:BOOL=${STATIC_BUILD} \
  -DSDK_BUILD_EXAMPLES=${SDK_BUILD_EXAMPLES} \
  -DSDK_BUILD_TESTS=${SDK_BUILD_TESTS} \
  -S . \
  -B ${BUILD_FOLDER} \
  -G Ninja \
  -DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS}" \
  -DCMAKE_TOOLCHAIN_FILE=${BUILD_FOLDER}/generators/conan_toolchain.cmake \
  .
  # ${XCOMPILE_TOOLCHAIN_FILE} ..
cmake --build ${BUILD_FOLDER} -v --config ${BUILD_TYPE} --target ${BUILD_TARGET}
