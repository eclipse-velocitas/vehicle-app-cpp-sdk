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
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy


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
        ("fmt/11.1.1"),
        ("grpc/1.67.1"),
        ("nlohmann_json/3.11.3"),
        ("paho-mqtt-c/1.3.13"),
        ("paho-mqtt-cpp/1.4.0"),
        ("zlib/1.3.1"),
    ]
    # generators = "CMakeToolchain","CMakeDeps"
    author = "Robert Bosch GmbH"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "STATIC_BUILD": [True, False],
        "SDK_BUILD_EXAMPLES": [True, False],
        "SDK_BUILD_TESTS": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "STATIC_BUILD": False,
        "SDK_BUILD_EXAMPLES": False,
        "SDK_BUILD_TESTS": False,
    }

    exports = "version.txt"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = (
        ".scripts/common.sh",
        "build.sh",
        "install_dependencies.sh",
        "CMakeLists.txt",
        "sdk/*",
        "examples/*",
        "conanfile.py",
        ".conan/profiles/*",
        "version.txt",
    )

    def set_version(self):
        try:
            tag = (
                subprocess.run(
                    ["git", "tag", "--points-at", "HEAD"], capture_output=True
                )
                .stdout.strip()
                .decode("utf-8")
            )
            version = ""
            if tag:
                version_tag_pattern = re.compile(r"^v[0-9]+(\.[0-9]+){0,2}")
                if version_tag_pattern.match(tag):
                    tag = tag[1:]  # cut off initial v if a semver tag
                version = tag

            # if no tag, use branch name or commit hash
            if not version:
                version = (
                    subprocess.run(
                        ["git", "symbolic-ref", "-q", "--short", "HEAD"],
                        capture_output=True,
                    )
                    .stdout.strip()
                    .decode("utf-8")
                )
            if not version:
                version = (
                    subprocess.run(
                        ["git", "rev-parse", "--short", "HEAD"], capture_output=True
                    )
                    .stdout.strip()
                    .decode("utf-8")
                )

            # / is not allowed in conan version
            version = version.replace("/", ".")
            open("./version.txt", mode="w", encoding="utf-8").write(version)
            self.version = version
        except:
            print("Not a git repository, reading version from static file...")
            if os.path.isfile("./version.txt"):
                self.version = open("./version.txt", encoding="utf-8").read().strip()
            else:
                raise FileNotFoundError("Missing version.txt!")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        # basic_layout(self, src_folder="sdk")
        cmake_layout(self, src_folder=".")

    # def source(self):
    #     get(self, **self.conan_data["sources"][self.version], strip_root=True)

    def generate(self):
        # This generates "conan_toolchain.cmake" in self.generators_folder
        tc = CMakeToolchain(self, generator="Ninja")
        tc.absolute_paths = True
        tc.cache_variables["STATIC_BUILD"] = self.options.STATIC_BUILD
        tc.cache_variables["SDK_BUILD_EXAMPLES"] = self.options.SDK_BUILD_EXAMPLES
        tc.cache_variables["SDK_BUILD_TESTS"] = self.options.SDK_BUILD_TESTS
        tc.generate()

        # This generates "foo-config.cmake" and "bar-config.cmake" in self.generators_folder
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        # build_type = self.settings.get_safe("build_type", default="Release").lower()
        # option = "-r" if build_type == "release" else "-d"
        # print(f"env: {os.environ}")
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        # subprocess.call(
        #    f"cd .. && ./install_dependencies.sh && ./build.sh {option} --no-examples --no-tests", shell=True, env=os.environ)

    def package(self):
        subprocess.call("pwd", shell=True)
        copy(self, "*.h", src="../sdk/include", dst="include", keep_path=True)
        copy(self, "*.h", src="../build/gens", dst="include", keep_path=True)
        copy(self, "*.a", src="../build/lib", dst="lib", keep_path=False)
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.bindirs = ["bin"]
        self.cpp_info.builddirs = ["cmake"]
        self.cpp_info.libs = ["vehicle-app-sdk", "vehicle-app-sdk-generated-grpc"]

    def imports(self):
        self.copy("license*", src=".", dst="./licenses", folder=True, ignore_case=True)

    def build_requirements(self):
        # 'build' context (protoc.exe will be available)
        self.tool_requires("grpc/1.67.1")
