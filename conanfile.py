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
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
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
    generators = "CMakeDeps"
    author = "Robert Bosch GmbH"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "STATIC_BUILD": ["ON", "OFF"],
        "SDK_BUILD_EXAMPLES": ["ON", "OFF"],
        "SDK_BUILD_TESTS": ["ON", "OFF"],
        "COVERAGE": ["ON", "OFF"],
        "BUILD_TARGET": ["ANY"],
        "BUILD_ARCH": ["ANY"],
        "HOST_ARCH": ["ANY"],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "STATIC_BUILD": "OFF",
        "SDK_BUILD_EXAMPLES": "OFF",
        "SDK_BUILD_TESTS": "OFF",
        "COVERAGE": "OFF",
        "BUILD_TARGET": "all",
        "BUILD_ARCH": os.uname().machine,
        "HOST_ARCH": os.uname().machine,
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
        # self.options.shared = self.options.STATIC_BUILD == "OFF"
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        self.options["grpc"].csharp_ext = False
        self.options["grpc"].php_plugin = False
        self.options["grpc"].node_plugin = False
        self.options["grpc"].otel_plugin = False
        self.options["grpc"].ruby_plugin = False
        self.options["grpc"].csharp_plugin = False
        self.options["grpc"].python_plugin = False
        self.options["grpc"].objective_c_plugin = False
        self.options["grpc"].fPIC = True
        # self.options["grpc"].shared = True
        # self.options["protobuf"].shared = True

    def layout(self):
        cmake_layout(self, src_folder=".")

    def generate(self):
        # This generates "conan_toolchain.cmake" in self.generators_folder
        cxx_flags = []
        # cxx_flags.append("-g")
        if self.settings.build_type == "Debug":
            cxx_flags.append("-O0")
        else:
            cxx_flags.append("-O3")
            cxx_flags.append("-s")

        if self.options.COVERAGE:
            cxx_flags.append("--coverage")

        tc = CMakeToolchain(self, generator="Ninja")
        tc.absolute_paths = True
        tc.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"
        tc.cache_variables["STATIC_BUILD"] = self.options.STATIC_BUILD
        tc.cache_variables["SDK_BUILD_EXAMPLES"] = self.options.SDK_BUILD_EXAMPLES
        tc.cache_variables["SDK_BUILD_TESTS"] = self.options.SDK_BUILD_TESTS
        tc.extra_cxxflags = cxx_flags
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(
            self,
            "*.h",
            src=os.path.join(self.source_folder, "sdk/include"),
            dst=os.path.join(self.package_folder, "include"),
            keep_path=True,
        )
        copy(
            self,
            "*.h",
            src=os.path.join(self.build_folder, "gens"),
            dst=os.path.join(self.package_folder, "include"),
            keep_path=True,
        )
        copy(
            self,
            "*.a",
            src=self.build_folder,
            dst=os.path.join(self.package_folder, "lib"),
            keep_path=False,
        )

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
