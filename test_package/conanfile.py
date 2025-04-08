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
from conan.tools.cmake import CMake, cmake_layout


class VehicleAppSdkTest(ConanFile):
    test_type = "explicit"
    name = "vehicle_app_sdk_test"
    license = "Apache-2.0"
    url = "https://github.com/eclipse-velocitas/vehicle-app-cpp-sdk"
    
    settings = "os", "compiler", "build_type", "arch"
    
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires(self.tested_reference_str)
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if can_run(self):
            cmd = os.path.join(self.cpp.build.bindir, "test_package")
            self.run(f"SDV_MIDDLEWARE_TYPE=native {cmd}", env="conanrun")
