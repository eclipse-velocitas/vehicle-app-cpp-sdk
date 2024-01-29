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
mode.

Arguments:
-d, --debug         Installs all dependencies in debug mode.
-r, --release       Installs all dependencies in release mode. (default)
-x, --cross <arch>  Cross compiles for the specified architecture.
--build-all-deps    Forces all dependencies to be rebuild from source.
-h, --help          Shows this help.
"
}

BUILD_VARIANT="release"
BUILD_ARCH=$(arch)
HOST_ARCH=${BUILD_ARCH}
WHICH_DEPS_TO_BUILD="missing"

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
    --build-all-deps)
      WHICH_DEPS_TO_BUILD="*"
      shift
      ;;
    -x|--cross)
      HOST_ARCH=$( get_valid_cross_compile_architecute "$2" )

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
echo "Build variant      ${BUILD_VARIANT}"
echo "Build arch         ${BUILD_ARCH}"
echo "Host arch          ${HOST_ARCH}"
echo "Building deps      ${WHICH_DEPS_TO_BUILD}"

mkdir -p build

XCOMPILE_PROFILE=""

if [[ "${BUILD_ARCH}" != "${HOST_ARCH}" ]]; then
  echo "Setting up cross compilation toolchain..."

  toolchain=/usr/bin/${HOST_ARCH}-linux-gnu
  target_host=${HOST_ARCH}-linux-gnu
  cc_compiler=gcc
  cxx_compiler=g++

  export CONAN_CMAKE_FIND_ROOT_PATH=$toolchain
  export CONAN_CMAKE_SYSROOT=$toolchain
  export CC=$target_host-$cc_compiler
  export CXX=$target_host-$cxx_compiler

  XCOMPILE_PROFILE="-pr:b .conan/profiles/linux_${BUILD_ARCH}_${BUILD_VARIANT}"
fi

# Enable Conan revision handling to enable pinning googleapis recipe revision (see conanfile.py)
export CONAN_REVISIONS_ENABLED=1

conan install \
    -pr:h .conan/profiles/linux_${HOST_ARCH}_${BUILD_VARIANT} \
    ${XCOMPILE_PROFILE} \
    --build "${WHICH_DEPS_TO_BUILD}" \
    -of ./build \
    -if ./build .
