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

from conans import ConanFile, tools
from conan.tools.cmake import cmake_layout, CMakeToolchain, CMake
import subprocess
import os
import re
import subprocess

from conan.tools.cmake import cmake_layout
from conans import ConanFile, tools



class VehicleAppCppSdkConan(ConanFile):
    name = "vehicle-app-sdk"
    license = "Apache-2.0"
    url = "https://github.com/eclipse-velocitas/vehicle-app-cpp-sdk"
    description = "The Vehicle App SDK for c++ allows to create Vehicle Apps from the Velocitas development model in the c++ programming language."
    # In general: Pin recipe revisions of dependencies having further dependencies to avoid build issues due to updated recipes
    # Workaround1: Pin recipe revision for transient dependency googleapis for enabling the container build
    # Workaround2: Pin recipe revision for transient dependency paho-mqtt-c cause latest is pulling libanl which cannot be found
    requires = [
        ("abseil/20230802.1", "override"),
        ("protobuf/3.21.12"),
        ("c-ares/1.19.1"),
        ("cpr/1.10.1"),
        ("fmt/9.1.0"),
        ("googleapis/cci.20221108"),
        ("grpc/1.50.1"),
        ("grpc-proto/cci.20220627"),
        ("libcurl/8.1.2"),
        ("nlohmann_json/3.11.2"),
        ("openssl/1.1.1u"),
        ("paho-mqtt-c/1.3.9"),
        ("paho-mqtt-cpp/1.2.0"),
        ("zlib/1.3"),
    ]
    generators = "cmake"
    author = "Robert Bosch GmbH"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "examples": [True, False]}
    default_options = {"shared": False, "examples": False}

    exports = "version.txt"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = ".scripts/common.sh", "build.sh", "install_dependencies.sh", "CMakeLists.txt", "sdk/*", "examples/*", "conanfile.py", ".conan/profiles/*", "version.txt"

    options = {"shared": [True, False]}
    
    default_options = {"shared": True}

    def set_version(self):
        try:
            git = tools.Git(folder=".")
            tag = git.get_tag()
            if tag is not None:
                version_tag_pattern = re.compile(r"^v[0-9]+(\.[0-9]+){0,2}")
                if version_tag_pattern.match(tag):
                    tag = tag[1:] # cut off initial v if a semver tag

            version = tag if tag is not None else git.get_branch()
            if version == "HEAD (no branch)":
                version = git.get_commit()
            self.version = version.replace("/", "_")
            open("./version.txt", mode="w", encoding="utf-8").write(self.version)
        except:
            print("Not a git repository, reading version from static file...")
            if os.path.isfile("./version.txt"):
                self.version = open("./version.txt", encoding="utf-8").read().strip()
            else:
                raise FileNotFoundError("Missing version.txt!")
                

    def config_options(self):
        if self.settings.os == "Linux":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self, src_folder="sdk")

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={
            "STATIC_BUILD:BOOL":not self.options["shared"],
            "SDK_BUILD_EXAMPLES:BOOL":self.options["examples"]
        })
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.bindirs = ["bin"]
        self.cpp_info.libs = ["vehicle-app-sdk",
                              "vehicle-app-sdk-generated-grpc"]

    def imports(self):
        self.copy("license*", src=".", dst="./licenses",
                  folder=True, ignore_case=True)

    def build_requirements(self):
        # 'build' context (protoc.exe will be available)
        self.tool_requires("grpc/1.50.1")
