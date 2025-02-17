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
-j <num_jobs>                    Number of parallel jobs to use for building. (default: 2)
-h, --help                       Shows this help.
"
}

BUILD_VARIANT=Debug
BUILD_ARCH=$(arch)
HOST_ARCH=${BUILD_ARCH}
BUILD_TARGET=all
STATIC_BUILD=OFF
SDK_BUILD_EXAMPLES=ON
SDK_BUILD_TESTS=ON
GEN_COVERAGE=OFF
WHICH_DEPS_TO_BUILD="missing"
NUM_JOBS=2

POSITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
  case $1 in
    -d|--debug)
      BUILD_VARIANT="Debug"
      shift
      ;;
    -r|--release)
      BUILD_VARIANT="Release"
      shift
      ;;
    -t|--target)
      BUILD_TARGET="$2"
      shift
      shift
      ;;
    --build-all-deps)
      WHICH_DEPS_TO_BUILD="*"
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
    -j)
      NUM_JOBS="$2"
      shift
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
echo "Host arch          ${HOST_ARCH}"
echo "Build target       ${BUILD_TARGET}"
echo "Build SDK tests    ${SDK_BUILD_TESTS}"
echo "Build SDK examples ${SDK_BUILD_EXAMPLES}"
echo "Static build       ${STATIC_BUILD}"
echo "Coverage           ${GEN_COVERAGE}"

CMAKE_CXX_FLAGS="-g -O0"

if [ "${BUILD_VARIANT}" == "release" ]; then
    CMAKE_CXX_FLAGS="-s -g -O3"
fi

if [ "${GEN_COVERAGE}" == "ON" ]; then
  CMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} --coverage"
fi

mkdir -p build
conan build . \
  -pr:a .conan/profiles/linux_${BUILD_VARIANT} \
  -s build_type=${BUILD_VARIANT} \
  -o:a="&:STATIC_BUILD=${STATIC_BUILD}" \
  -o:a="&:SDK_BUILD_EXAMPLES=${SDK_BUILD_EXAMPLES}" \
  -o:a="&:SDK_BUILD_TESTS=${SDK_BUILD_TESTS}" \
  -o:a="&:COVERAGE=${GEN_COVERAGE}" \
  -o:a="&:BUILD_TARGET=${BUILD_TARGET}" \
  -o:a="&:BUILD_ARCH=${BUILD_ARCH}" \
  -o:a="&:HOST_ARCH=${HOST_ARCH}" \
  --build $WHICH_DEPS_TO_BUILD \
  -c tools.build:jobs=$NUM_JOBS
