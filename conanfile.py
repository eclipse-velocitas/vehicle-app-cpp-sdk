# Copyright (c) 2022-2025 Contributors to the Eclipse Foundation
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

import os
import re
import subprocess

from conan import ConanFile
from conan.tools.build import cross_building
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.scm import Git



class VehicleAppCppSdkConan(ConanFile):
    name = "vehicle-app-sdk"
    license = "Apache-2.0"
    url = "https://github.com/eclipse-velocitas/vehicle-app-cpp-sdk"
    description = "The Vehicle App SDK for c++ allows to create Vehicle Apps from the Velocitas development model in the c++ programming language."
    # In general: Pin recipe revisions of dependencies having further dependencies to avoid build issues due to updated recipes
    # Workaround1: Pin recipe revision for transient dependency googleapis for enabling the container build
    # Workaround2: Pin recipe revision for transient dependency paho-mqtt-c cause latest is pulling libanl which cannot be found
    requires = [
        ("abseil/20240116.2"),
        ("bzip2/1.0.8"),
        ("c-ares/1.34.1"),
        ("cpr/1.11.0"),
        ("fmt/11.0.2"),
        ("grpc/1.67.1"),
        ("libcap/2.69"),
        ("libcurl/8.10.1"),
        ("libmount/2.39.2"),
        ("libselinux/3.6"),
        ("libsystemd/255.10"),
        ("libxcrypt/4.4.36"),
        ("lz4/1.9.4"),
        ("nlohmann_json/3.11.3"),
        ("openssl/3.3.2"),
        ("paho-mqtt-c/1.3.13"),
        ("paho-mqtt-cpp/1.4.0"),
        ("pcre2/10.42"),
        ("protobuf/5.27.0"),
        ("re2/20230301"),
        ("xz_utils/5.4.5"),
        ("zlib/1.3.1"),
        ("zstd/1.5.5"),
    ]
    generators = "CMakeDeps", "CMakeToolchain"
    author = "Robert Bosch GmbH"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    exports = "version.txt"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = ".scripts/common.sh", "build.sh", "install_dependencies.sh", "CMakeLists.txt", "sdk/*", "examples/*", "conanfile.py", ".conan/profiles/*", "version.txt"

    def set_version(self):
        print("########################## set_version ##########################")
        try:
            git = Git(folder=".")
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
        print("########################## config_options ##########################")
        if self.settings.get_safe("os") == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        print("########################## configure ##########################")
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def generate(self):
        print("########################## generate ##########################")

    def layout(self):
        print("########################## layout ##########################")
        cmake_layout(
            self, 
            src_folder="sdk",
            build_folder="build" if not cross_building(self) else f"build_{self.settings.os}_{self.settings.arch}",
        )

    def build(self):
        print("########################## build ##########################")
        build_type = self.settings.get_safe(
            "build_type", default="Release").lower()
        option = "-r" if build_type == "release" else "-d"
        subprocess.call(
            f"cd ../.. && ./install_dependencies.sh && ./build.sh {option} --no-examples --no-tests", shell=True)

    def package(self):
        print("########################## package ##########################")
        self.copy("*.h", src="../sdk/include", dst="include", keep_path=True)
        self.copy("*.h", src="../build/sdk/proto", dst="include", keep_path=True)
        self.copy("*.a", src="../build/lib", dst="lib", keep_path=False)

    def package_info(self):
        print("########################## package_info ##########################")
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.bindirs = ["bin"]
        self.cpp_info.libs = ["vehicle-app-sdk",
                              "vehicle-app-sdk-generated-grpc"]

    def imports(self):
        print("########################## imports ##########################")
        self.copy("license*", src=".", dst="./licenses",
                  folder=True, ignore_case=True)

    def build_requirements(self):
        print("########################## build_requirements ##########################")
        # 'build' context (protoc.exe will be available)
        self.tool_requires("grpc/<host_version>")
        self.tool_requires("protobuf/<host_version>")
