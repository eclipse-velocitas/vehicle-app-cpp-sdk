# Copyright (c) 2024-2025 Contributors to the Eclipse Foundation
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

from conan import ConanFile
from conan.tools.build import can_run
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


class VehicleAppSdkTest(ConanFile):
    test_type = "explicit"
    name = "vehicle_app_sdk_test"
    license = "Apache-2.0"
    url = "https://github.com/eclipse-velocitas/vehicle-app-cpp-sdk"

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

    generators = "CMakeDeps"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def generate(self):
        # This generates "conan_toolchain.cmake" in self.generators_folder
        tc = CMakeToolchain(self, generator="Ninja")
        tc.absolute_paths = True
        tc.generate()

    def layout(self):
        cmake_layout(self)

    def test(self):
        cmd = os.path.join(self.cpp.build.bindir, "test_package")
        if can_run(self):
            self.run(f"SDV_MIDDLEWARE_TYPE=native {cmd}", env="conanrun")
