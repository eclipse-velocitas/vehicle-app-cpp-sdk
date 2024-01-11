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

# syntax = docker/dockerfile:1.3

FROM alpine:3.16.0

RUN apk update && \
    apk add ninja && \
    apk add cmake && \
    apk add clang-extra-tools && \
    apk add python3 && \
    apk add py3-pip && \
    apk add build-base && \
    apk add linux-headers && \ 
    apk add perl && \
    apk add bash && \
    apk add git && \
    apk add nodejs && \
    apk add npm && \
    pip3 install "conan==1.60.2"

COPY . /sdk

RUN --mount=type=cache,target=/root/.conan

RUN cd /sdk && \
    ./install_dependencies.sh -r --build-all-deps && \
    cd / && rm -rf /sdk

# install CLI
RUN git clone https://github.com/eclipse-velocitas/cli.git && \ 
    cd cli && \
    npm i && npm run build && npx oclif manifest && npm i -g .
