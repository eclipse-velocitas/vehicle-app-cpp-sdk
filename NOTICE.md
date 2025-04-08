# Declared Project Licenses

This program and the accompanying materials are made available under the
terms of the Apache License, Version 2.0 which is available at
https://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
License for the specific language governing permissions and limitations
under the License.

SPDX-License-Identifier: Apache-2.0

## Contributors

* Robert Bosch GmbH

## Third-party Content

### Devcontainer

| Software                   | License                                                                                                                                                                      |
|:---------------------------|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Microsoft CPP DevContainer (including .devcontainer/scripts/reinstall-cmake.sh)| <https://github.com/microsoft/containerregistry/blob/main/legal/Container-Images-Legal-Notice.md>, <https://github.com/microsoft/vscode-dev-containers/blob/main/NOTICE.txt>, <https://github.com/microsoft/vscode-dev-containers/blob/main/LICENSE> |

### Development

|   Software   |                                    License                                     |
|:------------:|:------------------------------------------------------------------------------:|
|    CMake     |       <https://gitlab.kitware.com/cmake/cmake/raw/master/Copyright.txt>        |
|    Python    |                    <https://docs.python.org/3/license.html>                    |
|  Python Pip  |              <https://github.com/pypa/pip/blob/main/LICENSE.txt>               |
|    ccache    |                       <https://ccache.dev/license.html>                        |
|    gcovr     |                   <https://gcovr.com/en/stable/license.html>                   |
|    Conan     |          <https://github.com/conan-io/conan/blob/develop/LICENSE.md>           |
| clang-format | <https://github.com/llvm/llvm-project/blob/main/clang-tools-extra/LICENSE.TXT> |
|  clang-tidy  | <https://github.com/llvm/llvm-project/blob/main/clang-tools-extra/LICENSE.TXT> |
|    GTest     |            <https://github.com/google/googletest/blob/main/LICENSE>            |
| Kuksa Databroker<br>interface definition | https://github.com/eclipse-kuksa/kuksa-databroker/blob/0.5.0/LICENSE |

### Further 3rd party licenses used in project

[Auto-generated 3rd party licenses](./NOTICE-3RD-PARTY-CONTENT.md)

## C++

The C++ dependencies should be normally also listed in the auto-generated notice file linked above.
Due to the limited Conan support of the Pivotal License Finder currently used in our [License Check](https://github.com/eclipse-velocitas/license-check),
they are given here (manually added) for time being:

| Dependency | Version | License |
|:-----------|:-------:|--------:|
|abseil|20230802.1|Apache 2.0|
|c-ares|1.34.3|c-ares (MIT-style)|
|fmt|11.1.1|MIT|
|grpc|1.50.1|Apache 2.0|
|nlohmann_json|3.11.3|MIT|
|openssl|3.4.1|Apache 2.0|
|paho-mqtt-c|1.3.13|EPL 2.0 AND EDL 1.0|
|paho-mqtt-cpp|1.4.0|EPL 2.0 AND EDL 1.0|
|protobuf|3.21.12|Google License|
|re2|20230301|BSD-3-Clause|
|zlib|1.3.1|zlib license|
