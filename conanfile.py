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
    requires = [
        #("c-ares/1.19.1@#420a0b77e370f4b96bee88ef91837ccc"),
        ("fmt/9.1.0"),
        #("googleapis/cci.20221108@#e4bebdfa02f3b6f93bae1d5001b8d439"),
        ("grpc/1.50.1"),
        #("grpc/1.50.1@#df352027120f88bccf24cbc40a2297ce"),
        #("grpc-proto/cci.20220627@#3ad14e3ffdae516b4da2407d5f23c71d"),
        ("nlohmann_json/3.11.2"),
        #("openssl/1.1.1u@#de76bbea24d8b46f8def8daa18b31fd9"),
        ("paho-mqtt-c/1.3.13"),
        ("paho-mqtt-cpp/1.4.0"),
        #("zlib/1.3.1", "override")
    ]
    generators = "CMakeDeps", "CMakeToolchain"
    author = "Robert Bosch GmbH"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    exports = "version.txt"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = ".scripts/common.sh", "build.sh", "install_dependencies.sh", "CMakeLists.txt", "sdk/*", "examples/*", "conanfile.py", ".conan/profiles/*", "cmake/*", "version.txt"

    def set_version(self):
        print("########################## set_version ##########################")
        try:
            git = Git(self, folder=".")
            tag = git.run("tag --points-at HEAD").strip()
            if tag:
                version_tag_pattern = re.compile(r"^v[0-9]+(\.[0-9]+){0,2}")
                if version_tag_pattern.match(tag):
                    tag = tag[1:] # cut off initial v if a semver tag

            version = tag if tag else git.run("symbolic-ref -q --short HEAD").strip()
            if not version:
                version = git.get_commit(repository=True)
            self.version = version.replace("/", ".")
            open("./version.txt", mode="w", encoding="utf-8").write(self.version)
        except:
            print("Maybe not a git repository, reading version from static file...")
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

    def requirements(self):
        print("########################## requirements ##########################")

    def build_requirements(self):
        print("########################## build_requirements ##########################")
        # Declare both, grpc and protobuf, here to enable proper x-build (w/o using qemu)
        self.tool_requires("grpc/<host_version>")
        self.tool_requires("protobuf/<host_version>")

    def layout(self):
        print("########################## layout ##########################")
        os = str(self.settings.os).lower()
        arch = str(self.settings.arch).lower()
        if arch == "armv8":
            arch = "aarch64"
        cmake_layout(
            self, 
            src_folder="sdk",
            build_folder="build" if not cross_building(self) else f"build-{os}-{arch}",
        )

    def generate(self):
        print("########################## generate ##########################")
        print(f"{self.folders.source=}")
        print(f"{self.source_folder=}")
        print(f"{self.folders.build=}")
        print(f"{self.build_folder=}")
        print(f"{self.folders.generators=}")
        print(f"{self.folders.root=}")
        print(f"{self.folders.subproject=}")
        print(f"{self.folders.build_folder_vars=}")

    def build(self):
        print("########################## build ##########################")
        build_type = self.settings.get_safe(
            "build_type", default="Release").lower()
        option = "-r" if build_type == "release" else "-d"
        arch = str(self.settings.arch).lower()
        if arch == "armv8":
            arch = "aarch64"
        subprocess.call(
            f"cd ../.. && ./build.sh -x {self.settings.arch} {option} --no-examples --no-tests", shell=True)

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



    def build_id(self):
        print("########################## build_id ##########################")

    def compatibility(self):
        print("########################## compatibility ##########################")

    def build_id(self):
        print("########################## build_id ##########################")

    def deploy(self):
        print("########################## deploy ##########################")

    def export(self):
        print("########################## export ##########################")

    def export_sources(self):
        print("########################## export_sources ##########################")

    def finalize(self):
        print("########################## finalize ##########################")

    def init(self):
        print("########################## init ##########################")

    def package_id(self):
        print("########################## package_id ##########################")

    def set_name(self):
        print("########################## set_name ##########################")

    def source(self):
        print("########################## source ##########################")

    def system_requirements(self):
        print("########################## system_requirements ##########################")

    def test(self):
        print("########################## test ##########################")

    def validate(self):
        print("########################## validate ##########################")

    def validate_build(self):
        print("########################## validate_build ##########################")
