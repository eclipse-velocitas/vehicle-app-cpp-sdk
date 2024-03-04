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

import argparse
import json
import os
import shutil
import subprocess  # nosec B404
from pathlib import Path
from typing import Iterable, List


def get_repo_root() -> Path:
    return Path(os.path.dirname(__file__)).parent


def verbose_copy(src, dst) -> object:
    print(f"Copying {src!r} to {dst!r}")
    return shutil.copy2(src, dst)


def copy_files(root_destination: str) -> None:
    with open(f"{os.path.dirname(__file__)}/config.json") as f:
        files = json.load(f)["files"]
        for file in files:
            destination = root_destination

            if ".project-creation" in file:
                destination = os.path.join(
                    root_destination,
                    os.path.dirname(file.removeprefix(".project-creation/templates/")),
                )

            Path(destination).mkdir(parents=True, exist_ok=True)
            source = f"{get_repo_root()}/{file}"
            verbose_copy(source, destination)


def _filter_hidden_files(_: str, dir_contents: List[str]) -> Iterable[str]:
    hidden_files = [".git"]
    return filter(lambda file: file in hidden_files, dir_contents)


def copy_project(source_path: str, destination_repo: str) -> None:
    app_path = os.path.join(destination_repo, "app")

    shutil.copytree(
        source_path,
        app_path,
        copy_function=verbose_copy,
        dirs_exist_ok=True,
        ignore=_filter_hidden_files,
    )

    readme_path = os.path.join(app_path, "README.md")
    if os.path.exists(readme_path):
        existing_readme_path = os.path.join(destination_repo, "README.md")
        if os.path.exists(existing_readme_path):
            os.remove(existing_readme_path)
        shutil.move(readme_path, destination_repo, copy_function=verbose_copy)


def compile_requirements(destination_repo: str) -> None:
    subprocess.check_call(  # nosec B603, B607
        ["pip", "install", "pip-tools"], cwd=os.path.join(destination_repo, "app")
    )

    subprocess.check_call(  # nosec B603, B607
        ["python", "-m", "piptools", "compile"],
        cwd=os.path.join(destination_repo, "app"),
    )

    subprocess.check_call(  # nosec B603, B607
        ["python", "-m", "piptools", "compile"],
        cwd=os.path.join(destination_repo, "app", "tests"),
    )

    subprocess.check_call(  # nosec B603, B607
        ["python", "-m", "piptools", "compile"], cwd=destination_repo
    )


def main():
    parser = argparse.ArgumentParser("run")
    parser.add_argument(
        "-d",
        "--destination",
        type=str,
        required=True,
        help="Path to the root of the repository.",
    )
    parser.add_argument(
        "-e",
        "--example",
        type=str,
        required=False,
        help="Copy the given example to the new repo.",
    )
    args = parser.parse_args()

    copy_files(args.destination)

    examples_directory_path = os.path.join(get_repo_root(), "examples")
    example_app = (
        os.path.join(examples_directory_path, args.example)
        if args.example
        else os.path.join(get_repo_root(), ".project-creation", ".skeleton")
    )
    copy_project(example_app, args.destination)

    # compile_requirements(args.destination)


if __name__ == "__main__":
    main()
