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
# Installs the Conan dependencies of the Vehicle App SDK
# into the local Conan cache. Has to be re-executed whenever any conanfile.txt
# or conanfile.py is updated.
#

source ./.scripts/common.sh

function print_help() {
  echo "Install dependencies
============================================================================
Installs the Conan dependencies of the Vehicle App AND the Vehicle App SDK
into the local Conan cache. Has to be re-executed whenever any conanfile.txt
or conanfile.py is updated. By default, dependencies are installed in release
mode and the app is expected to be built in debug mode (i.e. "mixed" mode).

Arguments:
-d, --debug         Installs all dependencies in debug mode.
-r, --release       Installs all dependencies in release mode.
-x, --cross <arch>  Cross compiles for the specified architecture.
--build-all-deps    Forces all dependencies to be rebuild from source.
-h, --help          Shows this help.
"
}

BUILD_TYPE="Release"
PREPARE_MIXED_BUILD="true"
BUILD_ARCH=$(arch)
HOST_ARCH=${BUILD_ARCH}
WHICH_DEPS_TO_BUILD="missing"

while [[ $# -gt 0 ]]; do
  case $1 in
    -d|--debug)
      PREPARE_MIXED_BUILD="false"
      BUILD_TYPE="Debug"
      shift
      ;;
    -r|--release)
      PREPARE_MIXED_BUILD="false"
      BUILD_TYPE="Release"
      shift
      ;;
    --build-all-deps)
      WHICH_DEPS_TO_BUILD="*"
      shift
      ;;
    -x|--cross)
      PREPARE_MIXED_BUILD="false"
      HOST_ARCH=$( get_valid_cross_compile_architecture "$2" )

      if [ "$?" -eq 1 ]; then
        echo "Invalid cross-compile architecture '$2'!"
        exit 1
      fi

      shift
      shift
      ;;
    -h|--help)
      print_help
      exit 0
      shift
      ;;
    -*|--*)
      echo "Unknown option: $1"
      exit 1
      ;;
    *)
      echo "Unknown argument: $1"
      exit 1
      ;;
  esac
done

echo "Conan version      "`conan --version`
echo "Build type         ${BUILD_TYPE}"
echo "  prepare mixed    ${PREPARE_MIXED_BUILD}"
echo "Build arch         ${BUILD_ARCH}"
echo "Host arch          ${HOST_ARCH}"
echo "Building deps      ${WHICH_DEPS_TO_BUILD}"

HOST_PROFILE=".conan/profiles/linux-${HOST_ARCH}"
BUILD_PROFILE=".conan/profiles/linux-${BUILD_ARCH}"

if [ "${PREPARE_MIXED_BUILD}" == "true" ]; then
    echo "Installing dependencies for \"mixed\" build: Dependencies in Release mode, SDK/examples/tests in Debug mode ..."
    conan install \
        -pr:h ${HOST_PROFILE} \
        -pr:b ${BUILD_PROFILE} \
        -s:a="build_type=Release" \
        -s:h="&:build_type=Debug" \
        --build "${WHICH_DEPS_TO_BUILD}" \
        .
fi

echo "Installing dependencies for uniform build in ${BUILD_TYPE} mode ..."
conan install \
    -pr:h ${HOST_PROFILE} \
    -pr:b ${BUILD_PROFILE} \
    -s:a="build_type=${BUILD_TYPE}" \
    --build "${WHICH_DEPS_TO_BUILD}" \
    .
