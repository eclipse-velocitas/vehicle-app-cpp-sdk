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

# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.245.0/containers/cpp/.devcontainer/base.Dockerfile

# [Choice] Debian / Ubuntu version (use Debian 11, Ubuntu 18.04/22.04 on local arm64/Apple Silicon): debian-11, debian-10, ubuntu-22.04, ubuntu-20.04, ubuntu-18.04
ARG VARIANT="bullseye"
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE
ENV REINSTALL_CMAKE_VERSION_FROM_SOURCE="${REINSTALL_CMAKE_VERSION_FROM_SOURCE:-none}"

# Force dapr to use localhost traffic
ENV DAPR_HOST_IP="127.0.0.1"
# Add daprd to the path for the VS Code Dapr extension.
ENV PATH=$PATH:/home/vscode/.dapr/bin

# Redirect conan home to avoid cache eviction on container rebuild
ARG WORKSPACE_ROOT
ENV CONAN_USER_HOME="${WORKSPACE_ROOT}/.conan_home"

COPY scripts/*.sh /tmp/scripts/
RUN find /tmp/scripts/ -type f -iname "*.sh" -exec chmod +x {} \;
RUN /bin/bash /tmp/scripts/configure-proxies.sh
