#!/usr/bin/env bash
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

sudo chmod +x .devcontainer/scripts/*.sh
sudo chmod +x .vscode/scripts/runtime/k3d/*.sh
sudo chmod +x .vscode/scripts/runtime/local/*.sh
sudo chown -R $(whoami) $HOME

echo "#######################################################"
echo "### Configure Proxy                                 ###"
echo "#######################################################"
sudo .devcontainer/scripts/configure-proxies.sh 2>&1 | tee -a $HOME/configure-proxies.log

echo "#######################################################"
echo "### Install Prerequisites and Tools                 ###"
echo "#######################################################"

# Optionally install the cmake for vcpkg
.devcontainer/scripts/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}

# Install python, conan and ccache
sudo apt-get update
sudo apt-get -y install --no-install-recommends ccache
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 100

pip3 install -r ./requirements.txt

# Install static analyzer tools
sudo apt-get install -y cppcheck clang-format-14 clang-tidy-14
sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-14 100
sudo update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-14 100

./.devcontainer/scripts/ensure-dapr.sh 2>&1 | tee -a $HOME/ensure-dapr.log

if [ "${CODESPACES}" = "true" ]; then
    echo "#######################################################"
    echo "### Setup Access to Codespaces                      ###"
    echo "#######################################################"

    # Remove the default credential helper
    sudo sed -i -E 's/helper =.*//' /etc/gitconfig

    # Add one that just uses secrets available in the Codespace
    git config --global credential.helper '!f() { sleep 1; echo "username=${GITHUB_USER}"; echo "password=${MY_GH_TOKEN}"; }; f'
fi

echo "#######################################################"
echo "### Init submodules                                 ###"
echo "#######################################################"
git submodule update --init

echo "#######################################################"
echo "### Install Dependencies                            ###"
echo "#######################################################"
./install_dependencies.sh 2>&1 | tee -a $HOME/install_dependencies.log


