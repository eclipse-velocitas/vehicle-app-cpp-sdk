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

# Fix for bug in devcontainer features
ADD https://raw.githubusercontent.com/devcontainers/features/7fa90110d762797cc0b1c2fe8fcc028c9b813d56/src/common-utils/install.sh /tmp/scripts/install-common.sh
RUN UID="4000" bash /tmp/scripts/install-common.sh
ADD https://raw.githubusercontent.com/devcontainers/features/7fa90110d762797cc0b1c2fe8fcc028c9b813d56/src/docker-in-docker/install.sh /tmp/scripts/install-dind.sh
RUN VERSION="20.10" bash /tmp/scripts/install-dind.sh

VOLUME [ "/var/lib/docker" ]

# Setting the ENTRYPOINT to docker-init.sh will start up the Docker Engine
# inside the container "overrideCommand": false is set in devcontainer.json.
# The script will also execute CMD if you need to alter startup behaviors.
ENTRYPOINT [ "/usr/local/share/docker-init.sh" ]
CMD [ "sleep", "infinity" ]
