# Copyright (c) 2023 Robert Bosch GmbH

# This program and the accompanying materials are made available under the
# terms of the Apache License, Version 2.0 which is available at
# https://www.apache.org/licenses/LICENSE-2.0.

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.

# SPDX-License-Identifier: Apache-2.0

import os
import subprocess  # nosec
import time
import unittest

from parameterized import parameterized

devenv_runtimes_path = (
    subprocess.check_output(["velocitas", "package", "-p", "devenv-runtimes"])  # nosec
    .decode("utf-8")
    .strip("\n")
)


class RuntimeTest(unittest.TestCase):
    def _set_env(self, runtime: str) -> None:
        mqtt_port = 1883
        vdb_port = 55555

        if runtime == "runtime_k3d":
            mqtt_port = 31883
            vdb_port = 30555

        os.environ["MQTT_PORT"] = f"{mqtt_port}"
        os.environ["VDB_PORT"] = f"{vdb_port}"

    def _disable_mocking(self) -> None:
        with open("./mock.py", mode="w", encoding="utf-8") as file:
            file.write(
                """
from lib.dsl import mock_datapoint

# No mock configuration for integration tests.
mock_datapoint(
    path="Vehicle.VehicleIdentification.Model",
    initial_value="mock",
    behaviors=[]
)
"""
            )

    @parameterized.expand(["runtime_k3d", "runtime_kanto"])
    def test_runtime(self, runtime):
        self._set_env(runtime)
        self._disable_mocking()

        subprocess.check_call(  # nosec
            [
                "pytest",
                "-s",
                "-x",
                (
                    f"{devenv_runtimes_path}/{runtime}/test/integration/"
                    f"integration_test.py::test_scripts_run_successfully"
                ),
            ]
        )

        time.sleep(20)  # give app some time to be ready to receive mqtt messages

        # subprocess.check_call(  # nosec
        #     ["pytest", "-s", "-x", "./app/tests/integration/integration_test.py"]
        # )

        subprocess.check_call(  # nosec
            ["velocitas", "exec", runtime.replace("_", "-"), "down"]
        )
