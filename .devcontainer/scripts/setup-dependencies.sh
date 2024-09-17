#!/bin/bash
# This file is maintained by velocitas CLI, do not modify manually. Change settings in .velocitas.json
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
echo "### Install Prerequisites and Tools                 ###"
echo "#######################################################"

if [[ -z "${VELOCITAS_OFFLINE}" ]]; then
    # Optionally install the cmake for vcpkg
    .devcontainer/scripts/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}

    # Install python, conan and ccache
    sudo apt-get update
    sudo apt-get install -y python3
    sudo apt-get install -y python3-distutils
    curl -fsSL https://bootstrap.pypa.io/get-pip.py | sudo python3
    sudo apt-get -y install --no-install-recommends ccache

    build_arch=$(arch)

    # ensure we can always build for an arm target
    if [ "${build_arch}" != "aarch64" ]; then
        sudo apt-get install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
    fi

    pip3 install -r ./requirements.txt

    # Install static analyzer tools
    sudo apt-get install -y cppcheck clang-format-14 clang-tidy-14
    sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-14 100
    sudo update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-14 100
fi

echo "#######################################################"
echo "### Install Dependencies                            ###"
echo "#######################################################"
./install_dependencies.sh 2>&1 | tee -a $HOME/install_dependencies.log
