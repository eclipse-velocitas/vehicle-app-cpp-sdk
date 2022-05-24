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

BUILD_VARIANT="debug"
BUILD_ARCH=$(arch)
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

echo "Build variant ${BUILD_VARIANT}"
echo "Build arch    ${BUILD_ARCH}"
echo "Building deps ${WHICH_DEPS_TO_BUILD}"

mkdir -p build && cd build
conan install -pr:h ../.conan/profiles/linux_${BUILD_ARCH}_${BUILD_VARIANT} --build "${WHICH_DEPS_TO_BUILD}" ..