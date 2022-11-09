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


# Port of the local Proxy
ARG PROXY
ENV PROXY="${PROXY}"

ARG PROXY_HOST
ENV PROXY_HOST="${PROXY_HOST:-http://host.docker.internal}"

ARG PROXY_PORT
ENV PROXY_PORT="${PROXY_PORT:-3128}"

# Redirect conan home to avoid cache eviction on container rebuild
ARG WORKSPACE_ROOT
ENV CONAN_USER_HOME="${WORKSPACE_ROOT}/.conan_home"
